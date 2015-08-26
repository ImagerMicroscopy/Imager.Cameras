
#ifdef WITH_PHOTOMETRICS

#include "PhotometricsCamera.h"

#include <algorithm>

#include "PVCAM/master.h"
#include "PVCAM/pvcam.h"
#include "XOPStandardHeaders.h"

PhotometricsCamera::PhotometricsCamera(const std::string& cameraName) :
	_identifier(cameraName)
{
	int err = pl_cam_open(const_cast<char*>(cameraName.c_str()), &_pvcamHandle, OPEN_EXCLUSIVE);
	if (!err)
		throw std::runtime_error(_getPVCAMErrorMessage());
	err = pl_cam_get_diags(_pvcamHandle);
	if (!err)
		throw std::runtime_error(_getPVCAMErrorMessage());

	// default values

	this->setExposureTime(50e-3);
	this->setCoolerOnOff(false);
}

PhotometricsCamera::~PhotometricsCamera() {
	pl_cam_close(_pvcamHandle);
}

std::string PhotometricsCamera::getIdentifierStr() const {
	return _identifier;
}

bool PhotometricsCamera::setExposureTime(const double exposureTime) {
	// ensure camera is set to accept exposure time in milliseconds
	std::uint16_t exposureTimeResolution = EXP_RES_ONE_MILLISEC;
	int err = pl_set_param(_pvcamHandle, PARAM_EXP_RES_INDEX, &exposureTimeResolution);
	if (!err) {
		throw std::runtime_error(_getPVCAMErrorMessage());
	}

	_requestedExposureTime = exposureTime;
	_validateExposureTime();
	return (exposureTime == _requestedExposureTime);
}

bool PhotometricsCamera::setEMGain(const double emGain) {
	int result;

	_selectFastestReadoutPort(emGain != 0.0);
	if (emGain > 0) {
		std::uint16_t multFactor = emGain;
		result = pl_set_param(_pvcamHandle, PARAM_GAIN_MULT_FACTOR, &multFactor);
		if (!result)
			throw std::runtime_error(_getPVCAMErrorMessage());
	}
	return true;
}

bool PhotometricsCamera::setTemperature(const double temperature) {
	std::int16_t scaledSetPoint = temperature * 1000.0;
	int err = pl_set_param(_pvcamHandle, PARAM_TEMP_SETPOINT, &scaledSetPoint);
	if (err == 0) {
		throw std::runtime_error(_getPVCAMErrorMessage());
	}
	return true;
}

double PhotometricsCamera::getExposureTime() const {
	return _requestedExposureTime;
}

double PhotometricsCamera::getEMGain() const {
	int readoutPort;
	int err = pl_get_param(_pvcamHandle, PARAM_READOUT_PORT, ATTR_CURRENT, &readoutPort);
	if (!err) {
		// handle me
	}

	if (readoutPort != READOUT_PORT_MULT_GAIN)
		return 0.0;

	uint16_t gain;
	err = pl_get_param(_pvcamHandle, PARAM_GAIN_MULT_FACTOR, ATTR_CURRENT, &gain);
	if (!err) {
		// handle me
	}

	return gain;
}

double PhotometricsCamera::getTemperature() const {
	int16_t temperature;

	int err = pl_get_param(_pvcamHandle, PARAM_TEMP, ATTR_CURRENT, &temperature);
	if (err)
		return std::numeric_limits<double>::quiet_NaN();

	return (static_cast<double>(temperature) / 100.0);
}

std::pair<int, int> PhotometricsCamera::getSensorSize() const {
	int err;
	std::uint16_t nRows, nCols;
	err = pl_get_param(_pvcamHandle, PARAM_PAR_SIZE, ATTR_CURRENT, &nRows);
	err = pl_get_param(_pvcamHandle, PARAM_SER_SIZE, ATTR_CURRENT, &nCols);
	return std::pair<int, int>(nRows, nCols);
}

std::vector<uint16_t> PhotometricsCamera::acquireImages(const int nImages) {
	int err;
	std::pair<int, int> chipSize = this->getSensorSize();
	rgn_type region = { 0, chipSize.second - 1, 1, 0, chipSize.first - 1, 1 };
	int scaledExposureTime = _requestedExposureTime * 1.0e3;

	// check that the camera is ready to go
	err = pl_cam_get_diags(_pvcamHandle);
	if (err == 0) {
		throw std::runtime_error(_getPVCAMErrorMessage());
	}

	// init exposure functionality
	err = pl_exp_init_seq();
	if (err == 0)
		throw std::runtime_error(_getPVCAMErrorMessage());

	std::uint32_t requiredMemorySpace;
	err = pl_exp_setup_seq(_pvcamHandle, nImages, 1, &region, TIMED_MODE, scaledExposureTime, reinterpret_cast<uns32_ptr>(&requiredMemorySpace));
	if (err == 0) {
		throw std::runtime_error(_getPVCAMErrorMessage());
	}
	if (requiredMemorySpace != (chipSize.first * chipSize.second * nImages * sizeof(uint16_t))) {
		throw std::runtime_error("Memory size doesn't match");
	}

	std::vector<uint16_t> images(chipSize.first * chipSize.second * nImages);

	// start the acquisition
	err = pl_exp_start_seq(_pvcamHandle, reinterpret_cast<void*>(images.data()));
	if (err == 0) {
		throw std::runtime_error(_getPVCAMErrorMessage());
	}

	// wait until the camera has finished recording
	for ( ; ; ) {
		std::int16_t status;
		std::uint32_t nBytesRecorded;
		err = pl_exp_check_status(_pvcamHandle, &status, reinterpret_cast<uns32_ptr>(&nBytesRecorded));
		if (err == 0) {
			throw std::runtime_error(_getPVCAMErrorMessage());
		}

		if (status == READOUT_COMPLETE) {
			break;
		}

		// does the user wish to abort the exposure?
		int userAbort = CheckAbort(0);
		if (userAbort == -1) {
			err = pl_exp_abort(_pvcamHandle, CCS_NO_CHANGE);
			if (err == 0) {
				throw std::runtime_error(_getPVCAMErrorMessage());
			}
			break;
		}
	}

	return images;
}

void PhotometricsCamera::_selectFastestReadoutPort(bool useEMGain) {
	int result;
	if (useEMGain) {
		int readoutPort = READOUT_PORT_MULT_GAIN;
		result = pl_set_param(_pvcamHandle, PARAM_READOUT_PORT, &readoutPort);
		if (!result)
			throw std::runtime_error(_getPVCAMErrorMessage());
	} else {
		int readoutPort = READOUT_PORT_NORMAL;
		result = pl_set_param(_pvcamHandle, PARAM_READOUT_PORT, &readoutPort);
		if (!result)
			throw std::runtime_error(_getPVCAMErrorMessage());
	}

	std::int16_t nSpeedTableEntries;
	result = pl_get_param(_pvcamHandle, PARAM_SPDTAB_INDEX, ATTR_MAX, &nSpeedTableEntries);
	if (!result)
		throw std::runtime_error(_getPVCAMErrorMessage());

	std::int16_t fastestIndex = 0;
	std::uint16_t pixelTime, shortestPixelTime = -1;
	for (std::int16_t i = 0; i < nSpeedTableEntries; i++) {
		result = pl_set_param(_pvcamHandle, PARAM_SPDTAB_INDEX, &i);
		if (!result)
			throw std::runtime_error(_getPVCAMErrorMessage());

		std::int16_t bitDepth;
		result = pl_get_param(_pvcamHandle, PARAM_BIT_DEPTH, ATTR_CURRENT, &bitDepth);
		if (!result)
			throw std::runtime_error(_getPVCAMErrorMessage());
		if (bitDepth < 12)
			continue;

		result = pl_get_param(_pvcamHandle, PARAM_PIX_TIME, ATTR_CURRENT, &pixelTime);
		if (!result)
			throw std::runtime_error(_getPVCAMErrorMessage());

		if (pixelTime < shortestPixelTime) {
			shortestPixelTime = pixelTime;
			fastestIndex = i;
		}
	}

	result = pl_set_param(_pvcamHandle, PARAM_SPDTAB_INDEX, &fastestIndex);
	if (!result)
		throw std::runtime_error(_getPVCAMErrorMessage());
}

void PhotometricsCamera::_validateExposureTime() {
	double minExposureTime;
	int err = pl_get_param(_pvcamHandle, PARAM_EXP_MIN_TIME, ATTR_MIN, &minExposureTime);
	if (!err) {
		throw std::runtime_error(_getPVCAMErrorMessage());
	}

	// according to the PVCAM manual, the minimal exposure time should be reported in seconds,
	// but I kept getting a value of 1.0. So now I'm thinking this value is in milliseconds instead
	// (probably in units of PARAM_EXP_RES_INDEX).
	minExposureTime /= 1.0e3;

	_requestedExposureTime = std::max(_requestedExposureTime, minExposureTime);
	_requestedExposureTime = std::min(_requestedExposureTime, 10.0);
}

std::string PhotometricsCamera::_getPVCAMErrorMessage() const {
	char buf[ERROR_MSG_LEN];
	pl_error_message(pl_error_code(), buf);
	return std::string(buf);
}

#endif
