
#include "BaseCameraClass.h"

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

int BaseCameraClass::startAsyncAcquisition(bool freeRun, std::uint16_t* outputBuffer, int nImagesInBuffer) {
	if (_asyncIsRunning) {
		throw std::runtime_error("already running async");
	}
	if (_asyncWorkerThread.joinable()) {
		_asyncWorkerThread.join();
	}
	_asyncErrorCode = 0;
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
	_asyncIndexOfLastAcquisition = -1;

	_asyncIsRunning = true;
	_asyncWorkerThread = std::thread([&]() {
		_asyncAcquisitionWorker(freeRun, outputBuffer, nImagesInBuffer);
	});
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
	ScopedSetter<bool> runningResetter (&_asyncIsRunning, false);
	auto sensorSize = this->getSensorSize();
	int nPixelsInImage = sensorSize.first * sensorSize.second;

	_derivedStartAsyncAcquisition();

	int indexOfNextImage = 0;

	for ( ; ; ) {
		if (this->_asyncWantAbort) {
			_derivedAbortAsyncAcquisition();
			return;
		}

		while (!_derivedNewAsyncAcquisitionImageAvailable()) {
			if (this->_asyncWantAbort) {
				_derivedAbortAsyncAcquisition();
				return;
			}
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
}
