#include "SCConfigure.h"

#ifdef WITH_OCEANOPTICS

#include "OceanOpticsCamera.h"

#include "Utils.h"

OceanOpticsCamera::OceanOpticsCamera(long deviceID) :
	_seabreezeAPI(SeaBreezeAPI::getInstance()),
	_deviceID(deviceID)
{
	int errorCode = 0, err = 0;

	err = _seabreezeAPI->openDevice(_deviceID, &errorCode);
	if (err) {
		throw std::runtime_error("error opening ocean optics");
	}
	_spectrometerFeatures.resize(10);
	err = _seabreezeAPI->getSpectrometerFeatures(_deviceID, &errorCode, _spectrometerFeatures.data(), _spectrometerFeatures.size());
	if (err) {
		throw std::runtime_error("error getting spectrometer features");
	}
	_spectrumLength = _seabreezeAPI->spectrometerGetFormattedSpectrumLength(_deviceID, 0, &errorCode);
	if (errorCode != 0) {
		throw std::runtime_error("error getting spectrum length");
	}
	_minIntegrationTimeMicros = _seabreezeAPI->spectrometerGetMinimumIntegrationTimeMicros(_deviceID, 0, &errorCode);
	if (errorCode != 0) {
		throw std::runtime_error("error getting min exposure time");
	}
	_maxIntegrationTimeMicros = _seabreezeAPI->spectrometerGetMaximumIntegrationTimeMicros(_deviceID, 0, &errorCode);
	if (errorCode != 0) {
		throw std::runtime_error("error getting max exposure time");
	}
	_setDefaults();
}

OceanOpticsCamera::~OceanOpticsCamera() {
	int error = 0;
	_seabreezeAPI->closeDevice(_deviceID, &error);
}

std::string OceanOpticsCamera::getIdentifierStr() const {
	int err = 0, errorCode = 0;
	char buf[512];
	_seabreezeAPI->getDeviceType(_deviceID, &errorCode, buf, sizeof(buf));
	if (err) {
		throw std::runtime_error("error getting identifier string");
	}
	return std::string(buf);
}

void OceanOpticsCamera::_setDefaults() {
	_setExposureTime(0.1);
}

void OceanOpticsCamera::_setExposureTime(const double exposureTime) {
	int errorCode = 0;
	long integrationTimeMicros = static_cast<long>(exposureTime * 1.0e6);
	integrationTimeMicros = clamp(integrationTimeMicros, _minIntegrationTimeMicros, _maxIntegrationTimeMicros);
	_seabreezeAPI->spectrometerSetIntegrationTimeMicros(_deviceID, 0, &errorCode);
	if (errorCode != 0) {
		throw std::runtime_error("error setting exposure time");
	}
	_integrationTimeMicros = integrationTimeMicros;
}

double OceanOpticsCamera::_getExposureTime() const {
	return _integrationTimeMicros;
}

#endif