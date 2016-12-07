
#include "SCConfigure.h"

#ifdef WITH_DUMMYCAM

#include "DummyCamera.h"

#include <random>

DummyCamera::DummyCamera() :
	_exposureTime(50.0e-3),
	_emGain(5.0),
	_coolerOn(false),
	_temperature(-50.0),
	_frameCounter(0)
{

}

double DummyCamera::getTemperature() const {
	std::random_device rd;;
	std::default_random_engine randEngine(rd());
	std::uniform_real_distribution<double> uniformDist(-25.0, 25.0);
	return uniformDist(rd);
}

void DummyCamera::_derivedStartAsyncAcquisition() {
	_abortTimerThread = false;
	_shouldOfferNewImage = false;
	_timerThread = std::thread([=]() {
		std::int64_t exposureTimeMillis = this->getExposureTime() * 1000.0;
		for (;;) {
			if (this->_abortTimerThread)
				return;
			std::this_thread::sleep_for(std::chrono::milliseconds(exposureTimeMillis));
			this->_shouldOfferNewImage = true;
		}
	});
}
void DummyCamera::_derivedAbortAsyncAcquisition() {
	_abortTimerThread = true;
	if (_timerThread.joinable()) {
		_timerThread.join();
	}
}
bool DummyCamera::_derivedNewAsyncAcquisitionImageAvailable() {
	if (_shouldOfferNewImage) {
		_shouldOfferNewImage = false;
		return true;
	} else {
		return false;
	}
}
void DummyCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	int nPixels = nBytes / sizeof(std::uint16_t);
	for (int i = 0; i < nPixels; i++) {
		bufferForThisImage[i] = _frameCounter + i;
	}
	_frameCounter += 1;
}

#endif
