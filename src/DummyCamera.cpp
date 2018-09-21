
#include "SCConfigure.h"

#ifdef WITH_DUMMYCAM

#include "DummyCamera.h"

#include <random>

DummyCamera::DummyCamera() :
	_exposureTime(50.0e-3),
	_emGain(5.0),
	_coolerOn(false),
	_temperature(-50.0),
	_frameCounter(0) {

}

std::vector<CameraProperty> DummyCamera::getCameraProperties() {
	std::vector<CameraProperty> properties = getRequiredProperties();

	return properties;
}

void DummyCamera::setCameraProperty(const CameraProperty& prop) {
	if (setIfRequiredProperty(prop) == true) {
		return;
	}
	switch (prop.getPropertyType()) {
		default:
			throw std::runtime_error("setting unrecognized option");
	}
}

double DummyCamera::getFrameRate() const {
	return (1.0 / _getExposureTime());
}

void DummyCamera::_setExposureTime(const double exposureTime) {
	auto sensorSize = _getSensorSize();
	auto currentSize = getActualImageSize();
	double minExposureTime = 50e-3 / (sensorSize.first * sensorSize.second / (currentSize.first * currentSize.second));
	_exposureTime = clamp(exposureTime, minExposureTime, 500e-3);
}

std::shared_ptr<std::vector<uint16_t>> DummyCamera::_generateNewImage() {
    std::pair<int, int> sensorSize = _getSensorSize();
    int nPixels = sensorSize.first * sensorSize.second;
    std::shared_ptr<std::vector<uint16_t>> buf(new std::vector<uint16_t>(nPixels));
    uint16_t* vecPtr = buf->data();
    for (int i = 0; i < nPixels; i++) {
        vecPtr[i] = _frameCounter + i;
    }
    _frameCounter += 1;
    return buf;
}

void DummyCamera::_derivedStartAsyncAcquisition() {
	_abortTimerThread = false;
    while (!_imagesQueue.empty()) {
        _imagesQueue.pop();
    }
 	_timerThread = std::thread([=]() {
		std::int64_t exposureTimeMillis = this->_getExposureTime() * 1000.0;
		for (;;) {
			if (this->_abortTimerThread)
				return;
            std::this_thread::sleep_for(std::chrono::milliseconds(exposureTimeMillis));
            auto newImage = _generateNewImage();
            {
                std::lock_guard<std::mutex> guard(_imagesQueueMutex);
                _imagesQueue.push(newImage);
            }
		}
	});
}

void DummyCamera::_derivedAbortAsyncAcquisition() {
	_abortTimerThread = true;
	if (_timerThread.joinable()) {
		_timerThread.join();
	}
    while (!_imagesQueue.empty()) {
        _imagesQueue.pop();
    }
}

bool DummyCamera::_derivedNewAsyncAcquisitionImageAvailable() {
    {
        std::lock_guard<std::mutex> guard(_imagesQueueMutex);
        return (!_imagesQueue.empty());
    }
}
void DummyCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	int nPixelsInBuf = nBytes / sizeof(std::uint16_t);
    std::pair<int, int> sensorSize = _getSensorSize();
    int nPixels = sensorSize.first * sensorSize.second;
    if (nPixels != nPixelsInBuf) {
        throw std::runtime_error("_derivedStoreNewImageInBuffer() with incorrect buffer size");
    }
    std::shared_ptr<std::vector<uint16_t>> newImage;
    {
        std::lock_guard<std::mutex> guard(_imagesQueueMutex);
        newImage = _imagesQueue.front();
        _imagesQueue.pop();
    }
    memcpy(bufferForThisImage, newImage->data(), nPixels * sizeof(uint16_t));
}

#endif
