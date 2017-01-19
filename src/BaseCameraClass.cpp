
#include "BaseCameraClass.h"

#ifdef WITH_IGOR
	#include "XOPStandardHeaders.h"
#endif

template <typename T> 
class ScopedSetter{
public:
	ScopedSetter(T* valToModify, T value) :
		_valToModify(valToModify),
		_value(value)
	{}
	~ScopedSetter() {
		*_valToModify = _value;
	}
private:
	T* _valToModify;
	T _value;
};

BaseCameraClass::BaseCameraClass() :
	_asyncIsRunning(false)
{

}

BaseCameraClass::~BaseCameraClass() {
	if (_asyncIsRunning) {
		_asyncWantAbort = true;
	}
	if (_asyncWorkerThread.joinable()) {
		_asyncWorkerThread.join();
	}
}

void BaseCameraClass::setTemperature(const double temperature) {
	_derivedSetTemperature(temperature);
	_setCoolerOn(temperature < 15.0);
}

void BaseCameraClass::getAllowableExposureTimes(double* minExposureTime, double* maxExposureTime) {
	double currentExposureTime = getExposureTime();
	setExposureTime(1.0e-6);
	*minExposureTime = getExposureTime();
	*maxExposureTime = 1.0;
	setExposureTime(currentExposureTime);
}

void BaseCameraClass::getAllowableEMGains(double* minGain, double* maxGain) {
	std::pair<double, double> range = _derivedGetEMGainRange();
	*minGain = range.first;
	*maxGain = range.second;
}

void BaseCameraClass::acquireImages(const int nImages, std::uint16_t* outputBuffer) {
	startAsyncAcquisition(AcqFillAndStop, outputBuffer, nImages);
	while (_asyncIsRunning) {
		#ifdef WITH_IGOR
			if (SpinProcess()) {
				abortAsyncAquisition();
				return;
			}
		#endif
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

int BaseCameraClass::getAsyncStatus() {
	if (!_asyncErrorStr.empty()) {
		return -1;
	}
	if (!_asyncIsRunning) {
		return 0;
	} else {
		return 1;
	}
}

int BaseCameraClass::startAsyncAcquisition(AcquisitionMode acqMode, std::uint16_t* outputBuffer, int nImagesInBuffer) {
	if (_asyncIsRunning) {
		throw std::runtime_error("already running async");
	}
	if (_asyncWorkerThread.joinable()) {
		_asyncWorkerThread.join();
	}
	_asyncErrorStr.clear();
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
	_imageIndicesWaitingToBeCopied.clear();

	_asyncIsRunning = true;
	_asyncWorkerThread = std::thread([=]() {
		_asyncAcquisitionWorker(acqMode, outputBuffer, nImagesInBuffer);
	});

	return 0;
}

bool BaseCameraClass::isAsyncAcquisitionRunning() {
	return _asyncIsRunning;
}

void BaseCameraClass::abortAsyncAquisition() {
	_asyncWantAbort = true;
	_asyncWorkerThread.join();
}

int BaseCameraClass::getNImagesAsyncAcquired() {
	return _asyncNImagesStored;
}

int BaseCameraClass::getIndexOfLastImageAsyncAcquired() {
	std::lock_guard<std::mutex> lock(this->_imageIndicesMutex);
	if (_imageIndicesWaitingToBeCopied.empty()) {
		return -1;
	} else {
		int index = _imageIndicesWaitingToBeCopied.front();
		_imageIndicesWaitingToBeCopied.erase(_imageIndicesWaitingToBeCopied.begin());
		return index;
	}
}

std::pair<double, double> BaseCameraClass::_derivedGetEMGainRange() {
	double currentGain = getEMGain();
	double minGain = 0.0;
	setEMGain(1.0e12);
	double maxGain = getEMGain();
	setEMGain(currentGain);
	return std::pair<double, double>(minGain, maxGain);
}

void BaseCameraClass::_asyncAcquisitionWorker(AcquisitionMode acqMode, std::uint16_t* outputBuffer, int nImagesInBuffer) {
	ScopedSetter<bool> runningResetter(&_asyncIsRunning, false);
	auto sensorSize = this->getSensorSize();
	int nPixelsInImage = sensorSize.first * sensorSize.second;

	try {
		_derivedStartAsyncAcquisition();

		int indexOfNextImage = 0;

		for (;;) {
			if (this->_asyncWantAbort) {
				_derivedAbortAsyncAcquisition();
				return;
			}

			while (_derivedNewAsyncAcquisitionImageAvailable()) {
				if (this->_asyncWantAbort) {
					_derivedAbortAsyncAcquisition();
					return;
				}
				_derivedStoreNewImageInBuffer(outputBuffer + nPixelsInImage * indexOfNextImage, nPixelsInImage * sizeof(std::uint16_t));
				_asyncNImagesStored += 1;
				{
					std::lock_guard<std::mutex> lock(this->_imageIndicesMutex);
					if (_imageIndicesWaitingToBeCopied.size() == nImagesInBuffer) {
						// buffer overflow
						_imageIndicesWaitingToBeCopied.clear();
					}
					_imageIndicesWaitingToBeCopied.push_back(indexOfNextImage);
				}
				if ((acqMode == AcqFillAndStop) && (_asyncNImagesStored == nImagesInBuffer)) {
					_derivedAbortAsyncAcquisition();
					return;
				}

				indexOfNextImage = (indexOfNextImage + 1) % nImagesInBuffer;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
	catch (std::runtime_error& e) {
		_derivedAbortAsyncAcquisition();
		_asyncErrorStr = e.what();
		return;
	}
	catch (...) {
		_derivedAbortAsyncAcquisition();
		_asyncErrorStr = "unknown exception in async";
		return;
	}
}
