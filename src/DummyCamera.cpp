
#include "SCConfigure.h"

#ifdef WITH_DUMMYCAM

#include "DummyCamera.h"

#include <random>

DummyCamera::DummyCamera() :
	_exposureTime(50.0e-3),
	_emGain(5.0),
	_coolerOn(false),
	_temperature(-50.0)
{

}

double DummyCamera::getTemperature() const {
	std::random_device rd;;
	std::default_random_engine randEngine(rd());
	std::uniform_real_distribution<double> uniformDist(-25.0, 25.0);
	return uniformDist(rd);
}

void DummyCamera::_derivedStartAsyncAcquisition() {
}
void DummyCamera::_derivedAbortAsyncAcquisition() {
}
bool DummyCamera::_derivedNewAsyncAcquisitionImageAvailable() {
	return true;
}
void DummyCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	int nPixels = nBytes / sizeof(std::uint16_t);
	std::random_device rd;
	std::default_random_engine randEngine(rd());
	std::uniform_int_distribution<std::uint16_t> uniformDist(0, 65535);
	for (int i = 0; i < nPixels; i++) {
		bufferForThisImage[i] = uniformDist(randEngine);
	}
}

#endif
