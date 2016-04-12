
#include "BaseCameraClass.h"

int BaseCameraClass::startAsynAcquisition(bool freeRun, std::uint16_t* outputBuffer, int nImagesInBuffer) {
	_asyncErrorCode = 0;
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
	_asyncIndexOfLastAcquisition = -1;

	_asyncWorkerThread = std::thread([&]() {
		_asyncAcquisitionWorker(freeRun, outputBuffer, nImagesInBuffer);
	});
}

bool BaseCameraClass::isAsyncAcquisitionRunning() {
	return _derivedIsAsyncAcquisitionRunning();	// unsafe - may still be starting up
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
	auto sensorSize = this->getSensorSize();
	int nPixelsInImage = sensorSize.first * sensorSize.second;

	_derivedStartAsyncAcquisition();
	if (!_derivedIsAsyncAcquisitionRunning()) {
		_asyncErrorCode = 1;
		return;
	}

	int indexOfNextImage = 0;

	for ( ; ; ) {
		if (this->_asyncWantAbort) {
			_derivedAbortAsyncAcquisition();
			return;
		}

		while (!_derivedNewAsyncAcquisitionImageAvailable()) {

		}

		_derivedStoreNewImageInBuffer(outputBuffer + nPixelsInImage * indexOfNextImage);
		_asyncNImagesStored += 1;
		_asyncIndexOfLastAcquisition = indexOfNextImage;
		if (!freeRun && (_asyncNImagesStored == nImagesInBuffer)) {
			_derivedAbortAsyncAcquisition();
			return;
		}

		indexOfNextImage = (indexOfNextImage + 1) % nImagesInBuffer;
	}
}

int BaseCameraClass::getIndexOfLastImageAsyncAcquired() {
	return _asyncIndexOfLastAcquisition;
}
