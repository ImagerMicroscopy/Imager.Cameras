
#ifdef WITH_DUMMYCAM

#include "DummyCamera.h"

DummyCamera::DummyCamera() :
	_exposureTime(50.0e-3),
	_emGain(5.0),
	_coolerOn(false),
	_temperature(-50.0)
{

}

std::vector<uint16_t> DummyCamera::acquireImages(const int nImages) {
	std::pair<int, int> sensorSize = getSensorSize();

	int nPixels = sensorSize.first * sensorSize.second * nImages;
	std::vector<uint16_t> images(nPixels);

	for (int i = 0; i < nPixels; i++) {
		images[i] = (i % 65535);
	}

	return images;
}

#endif
