
#include "SCConfigure.h"

#ifdef WITH_PHOTOMETRICS

#include "PhotometricsCamera.h"

#include <chrono>
#include <algorithm>
#include <thread>

#include "Utils.h"
#include "PVCAM/master.h"
#include "PVCAM/pvcam.h"

#ifdef WITH_IGOR
	#include "XOPStandardHeaders.h"
#endif

PhotometricsCamera::PhotometricsCamera(const std::string& cameraName) :
	_identifier(cameraName)
{
	int err = pl_cam_open(const_cast<char*>(cameraName.c_str()), &_pvcamHandle, OPEN_EXCLUSIVE);
	if (!err)
		throw std::runtime_error(getPVCAMErrorMessage());
	err = pl_cam_get_diags(_pvcamHandle);
	if (!err)
		throw std::runtime_error(getPVCAMErrorMessage());

	// default values

	this->setExposureTime(50e-3);
	this->setCoolerOn(false);
}

PhotometricsCamera::~PhotometricsCamera() {
	pl_cam_close(_pvcamHandle);
}

std::string PhotometricsCamera::getIdentifierStr() const {
	return _identifier;
}

void PhotometricsCamera::setExposureTime(const double exposureTime) {
	// ensure camera is set to accept exposure time in milliseconds
	std::uint16_t exposureTimeResolution = EXP_RES_ONE_MILLISEC;
	int err = pl_set_param(_pvcamHandle, PARAM_EXP_RES_INDEX, &exposureTimeResolution);
	if (!err) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}

	_requestedExposureTime = exposureTime;
	_validateExposureTime();
}

void PhotometricsCamera::setEMGain(const double emGain) {
	int result;

	_selectFastestReadoutPort(emGain > 0.0);
	if (emGain > 0.0) {
		std::uint16_t minGain, maxGain;
		std::uint16_t multFactor = emGain;
		result = pl_get_param(_pvcamHandle, PARAM_GAIN_MULT_FACTOR, ATTR_MIN, &minGain);
		if (!result)
			throw std::runtime_error(getPVCAMErrorMessage());
		result = pl_get_param(_pvcamHandle, PARAM_GAIN_MULT_FACTOR, ATTR_MAX, &maxGain);
		if (!result)
			throw std::runtime_error(getPVCAMErrorMessage());
		multFactor = clamp(multFactor, minGain, maxGain);
		result = pl_set_param(_pvcamHandle, PARAM_GAIN_MULT_FACTOR, &multFactor);
		if (!result)
			throw std::runtime_error(getPVCAMErrorMessage());
	}
}

bool PhotometricsCamera::setTemperature(const double temperature) {
	std::int16_t scaledSetPoint = temperature * 100.0;
	std::int16_t minTemp, maxTemp;

	int err = pl_get_param(_pvcamHandle, PARAM_TEMP_SETPOINT, ATTR_MIN, &minTemp);
	if (!err)
		return false;
	err = pl_get_param(_pvcamHandle, PARAM_TEMP_SETPOINT, ATTR_MAX, &maxTemp);
	if (!err)
		return false;

	scaledSetPoint = std::max(std::min(scaledSetPoint, maxTemp), minTemp);

	err = pl_set_param(_pvcamHandle, PARAM_TEMP_SETPOINT, &scaledSetPoint);
	if (err == 0) {
		throw std::runtime_error(getPVCAMErrorMessage());
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
	if (!err)
		return std::numeric_limits<double>::quiet_NaN();

	return (static_cast<double>(temperature) / 100.0);
}

double PhotometricsCamera::getTemperatureSetpoint() const {
	std::int16_t setpoint;

	int err = pl_get_param(_pvcamHandle, PARAM_TEMP_SETPOINT, ATTR_CURRENT, &setpoint);
	if (!err)
		return std::numeric_limits<double>::quiet_NaN();

	return (static_cast<double>(setpoint) / 100.0);
}

std::pair<int, int> PhotometricsCamera::getSensorSize() const {
	int err;
	std::uint16_t nRows, nCols;
	err = pl_get_param(_pvcamHandle, PARAM_PAR_SIZE, ATTR_CURRENT, &nRows);
	err = pl_get_param(_pvcamHandle, PARAM_SER_SIZE, ATTR_CURRENT, &nCols);
	return std::pair<int, int>(nRows, nCols);
}

void PhotometricsCamera::_selectFastestReadoutPort(bool useEMGain) {
	int result;
	if (useEMGain) {
		int readoutPort = READOUT_PORT_MULT_GAIN;
		result = pl_set_param(_pvcamHandle, PARAM_READOUT_PORT, &readoutPort);
		if (!result)
			throw std::runtime_error(getPVCAMErrorMessage());
	} else {
		int readoutPort = READOUT_PORT_NORMAL;
		result = pl_set_param(_pvcamHandle, PARAM_READOUT_PORT, &readoutPort);
		if (!result)
			throw std::runtime_error(getPVCAMErrorMessage());
	}

	std::int16_t nSpeedTableEntries;
	result = pl_get_param(_pvcamHandle, PARAM_SPDTAB_INDEX, ATTR_MAX, &nSpeedTableEntries);
	if (!result)
		throw std::runtime_error(getPVCAMErrorMessage());

	std::int16_t fastestIndex = 0;
	std::uint16_t pixelTime, shortestPixelTime = -1;
	for (std::int16_t i = 0; i < nSpeedTableEntries; i++) {
		result = pl_set_param(_pvcamHandle, PARAM_SPDTAB_INDEX, &i);
		if (!result)
			throw std::runtime_error(getPVCAMErrorMessage());

		std::int16_t bitDepth;
		result = pl_get_param(_pvcamHandle, PARAM_BIT_DEPTH, ATTR_CURRENT, &bitDepth);
		if (!result)
			throw std::runtime_error(getPVCAMErrorMessage());
		if (bitDepth < 12)
			continue;

		result = pl_get_param(_pvcamHandle, PARAM_PIX_TIME, ATTR_CURRENT, &pixelTime);
		if (!result)
			throw std::runtime_error(getPVCAMErrorMessage());

		if (pixelTime < shortestPixelTime) {
			shortestPixelTime = pixelTime;
			fastestIndex = i;
		}
	}

	result = pl_set_param(_pvcamHandle, PARAM_SPDTAB_INDEX, &fastestIndex);
	if (!result)
		throw std::runtime_error(getPVCAMErrorMessage());
}

void PhotometricsCamera::_validateExposureTime() {
	double minExposureTime;
	int err = pl_get_param(_pvcamHandle, PARAM_EXP_MIN_TIME, ATTR_MIN, &minExposureTime);
	if (!err) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}

	// according to the PVCAM manual, the minimal exposure time should be reported in seconds,
	// but I kept getting a value of 1.0. So now I'm thinking this value is in milliseconds instead
	// (probably in units of PARAM_EXP_RES_INDEX).
	minExposureTime /= 1.0e3;
	_requestedExposureTime = clamp(_requestedExposureTime, minExposureTime, 1.0);
}

std::string PhotometricsCamera::getPVCAMErrorMessage() {
	char buf[ERROR_MSG_LEN];
	pl_error_message(pl_error_code(), buf);
	return std::string(buf);
}

void PhotometricsCamera::_derivedStartAsyncAcquisition() {
	int err = 0;
	std::pair<int, int> chipSize = this->getSensorSize();
	rgn_type region = { 0, chipSize.second - 1, 1, 0, chipSize.first - 1, 1 };
	int scaledExposureTime = _requestedExposureTime * 1.0e3;
	std::uint32_t nBytesInImage;
	int nImagesInBuffer = 10;

	// check that the camera is ready to go
	err = pl_cam_get_diags(_pvcamHandle);
	if (err == 0) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}

	// init exposure functionality
	err = pl_exp_init_seq();
	if (err == 0)
		throw std::runtime_error(getPVCAMErrorMessage());

	err = pl_exp_setup_cont(_pvcamHandle, 1, &region, TIMED_MODE, scaledExposureTime, reinterpret_cast<uns32_ptr>(&nBytesInImage), CIRC_OVERWRITE);
	if (err == 0) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}

	_asyncBuffer.resize(nBytesInImage * nImagesInBuffer);
	pl_exp_start_cont(_pvcamHandle, _asyncBuffer.data(), _asyncBuffer.size() * sizeof(std::uint16_t));
	if (err == 0) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}
}

void PhotometricsCamera::_derivedAbortAsyncAcquisition() {
	int err = pl_exp_stop_cont(_pvcamHandle, CCS_HALT);
	if (err == 0) {
		err = pl_exp_uninit_seq();
	}
}

bool PhotometricsCamera::_derivedNewAsyncAcquisitionImageAvailable() {
	int err = 0;
	std::int16_t status;
	std::uint32_t unused;
	err = pl_exp_check_cont_status(_pvcamHandle, &status, reinterpret_cast<uns32_ptr>(&unused), reinterpret_cast<uns32_ptr>(&unused));
	if (err == 0) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}
	if (status == READOUT_FAILED) {
		throw std::runtime_error("readout failed");
	}
	return (status == READOUT_COMPLETE);
}

void PhotometricsCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	uint16_t* address;
	int err = pl_exp_get_latest_frame(_pvcamHandle, reinterpret_cast<void**>(&address));
	if (err == 0) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}

	memcpy(bufferForThisImage, address, nBytes);
}

#endif
