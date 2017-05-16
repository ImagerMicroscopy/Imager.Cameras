
#include "BaseCameraClass.h"

#include <chrono>

#ifdef WITH_IGOR
	#include "XOPStandardHeaders.h"
#endif

#include "Utils.h"

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

BaseCameraClass::BaseCameraClass() {

}

BaseCameraClass::~BaseCameraClass() {
    abortAsyncAquisitionIfRunning();
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

void BaseCameraClass::acquireImages(const int nImages, const unsigned int nImagesToAverage, std::uint16_t* outputBuffer) {
	startAsyncAcquisition(AcqFillAndStop, nImagesToAverage, outputBuffer, nImages);
    for (;;) {
        std::future_status status = waitForAsyncAcquisitionEnd(100);
        if (status == std::future_status::ready) {
            _asyncWorkerFuture.get();
            if (!_asyncErrorStr.empty()) {
                throw std::runtime_error(_asyncErrorStr);
            } else {
                return;
            }
        }
        #ifdef WITH_IGOR
        if (SpinProcess()) {
            abortAsyncAquisitionIfRunning();
            return;
        }
        #endif
	}
}

int BaseCameraClass::getAsyncStatus() {
	if (!_asyncErrorStr.empty()) {
		return -1;
	}
    if (!isAsyncAcquisitionRunning()) {
		return 0;
	} else {
		return 1;
	}
}

int BaseCameraClass::startAsyncAcquisition(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer) {
    abortAsyncAquisitionIfRunning();
	_asyncErrorStr.clear();
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
	_imageIndicesWaitingToBeCopied.clear();

    _asyncWorkerFuture = std::async(std::launch::async, [=]() {
		_asyncAcquisitionWorker(acqMode, nImagesToAverage, outputBuffer, nImagesInBuffer);
	});

	return 0;
}

bool BaseCameraClass::wasAsyncAcquisitionStarted() const {
    return (_asyncWorkerFuture.valid());
}

bool BaseCameraClass::isAsyncAcquisitionRunning() const {
    return (wasAsyncAcquisitionStarted() && (_asyncWorkerFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout));
}

std::future_status BaseCameraClass::waitForAsyncAcquisitionEnd(int timeoutMillis) {
    if (!wasAsyncAcquisitionStarted()) {
        throw std::runtime_error("BaseCameraClass::waitForAsyncAcquisitionEnd() but no async acquisition has been started");
    }
    return _asyncWorkerFuture.wait_for(std::chrono::milliseconds(timeoutMillis));
}

void BaseCameraClass::abortAsyncAquisitionIfRunning() {
    if (wasAsyncAcquisitionStarted()) {
        _asyncWantAbort = true;
        _asyncWorkerFuture.get();
    }
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

void BaseCameraClass::_asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer) {
	auto sensorSize = this->getSensorSize();
	int nPixelsInImage = sensorSize.first * sensorSize.second;

	try {
		std::vector<std::uint16_t> singleImage;
		std::vector<std::uint32_t> avgImage;
		if (nImagesToAverage > 1) {
			singleImage.resize(nPixelsInImage);
			avgImage.resize(nPixelsInImage);
			memset(avgImage.data(), 0, avgImage.size() * sizeof(avgImage[0]));
		}

		_derivedStartAsyncAcquisition();
        CleanupRunner runner([&]() {
            this->_derivedAbortAsyncAcquisition();
        });

		std::uint32_t nImagesAccumulatedInAvg = 0;
		int indexOfNextImage = 0;

		for ( ; ;) {
			if (this->_asyncWantAbort) {
				return;
			}

            bool haveImage = _waitForNewImageWithTimeout(100);
            if (haveImage) {
				if (nImagesToAverage <= 1) {
					_derivedStoreNewImageInBuffer(outputBuffer + nPixelsInImage * indexOfNextImage, nPixelsInImage * sizeof(std::uint16_t));
				} else {
					_derivedStoreNewImageInBuffer(singleImage.data(), singleImage.size() * sizeof(singleImage[0]));
					for (int i = 0; i < nPixelsInImage; i += 1) {
						avgImage[i] += singleImage[i];
					}
					nImagesAccumulatedInAvg += 1;
					if (nImagesAccumulatedInAvg == nImagesToAverage) {
						for (int i = 0; i < nPixelsInImage; i += 1) {
							outputBuffer[nPixelsInImage * indexOfNextImage + i] = avgImage[i] / nImagesAccumulatedInAvg;
						}
						nImagesAccumulatedInAvg = 0;
						memset(avgImage.data(), 0, avgImage.size() * sizeof(avgImage[0]));
					} else {
						continue;
					}
				}

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
					return;
				}

				indexOfNextImage = (indexOfNextImage + 1) % nImagesInBuffer;
			}
		}
	}
	catch (std::runtime_error& e) {
		_asyncErrorStr = e.what();
		return;
	}
	catch (...) {
		_asyncErrorStr = "unknown exception in async";
		return;
	}
}

bool BaseCameraClass::_waitForNewImageWithTimeout(int timeoutMillis) {
    auto start = std::chrono::high_resolution_clock::now();
    for ( ; ; ) {
        if (_derivedNewAsyncAcquisitionImageAvailable()) {
            return true;
        }
        std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
        if (diff.count() > ((double)timeoutMillis / 1000.0)) {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
