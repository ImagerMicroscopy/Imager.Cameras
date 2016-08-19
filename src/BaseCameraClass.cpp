
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
	if (temperature > 15.0)
		_setCoolerOn(false);
}

void BaseCameraClass::getAllowableExposureTimes(double* minExposureTime, double* maxExposureTime) {
	double currentExposureTime = getExposureTime();
	setExposureTime(1.0e-6);
	*minExposureTime = getExposureTime();
	*maxExposureTime = 1.0;
	setExposureTime(currentExposureTime);
}

void BaseCameraClass::getAllowableEMGains(double* minGain, double* maxGain) {
	double currentGain = getEMGain();
	*minGain = 0.0;
	setEMGain(1.0e12);
	*maxGain = getEMGain();
	setEMGain(currentGain);
}

void BaseCameraClass::acquireImages(const int nImages, std::uint16_t* outputBuffer) {
	startAsyncAcquisition(false, outputBuffer, nImages);
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

int BaseCameraClass::startAsyncAcquisition(bool freeRun, std::uint16_t* outputBuffer, int nImagesInBuffer) {
	if (_asyncIsRunning) {
		throw std::runtime_error("already running async");
	}
	if (_asyncWorkerThread.joinable()) {
		_asyncWorkerThread.join();
	}
	_asyncErrorStr.clear();
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
	_asyncIndexOfLastAcquisition = -1;

	_asyncIsRunning = true;
	_asyncWorkerThread = std::thread([&]() {
		_asyncAcquisitionWorker(freeRun, outputBuffer, nImagesInBuffer);
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
	return _asyncIndexOfLastAcquisition;
}

void BaseCameraClass::_asyncAcquisitionWorker(bool freeRun, std::uint16_t* outputBuffer, int nImagesInBuffer) {
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
				_asyncIndexOfLastAcquisition = indexOfNextImage;
				if (!freeRun && (_asyncNImagesStored == nImagesInBuffer)) {
					_derivedAbortAsyncAcquisition();
					return;
				}

				indexOfNextImage = (indexOfNextImage + 1) % nImagesInBuffer;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
