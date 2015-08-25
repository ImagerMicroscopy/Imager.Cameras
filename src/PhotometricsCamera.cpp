
#include "PhotometricsCamera.h"
#include "PVCAM/master.h"
#include "PVCAM/pvcam.h"
#include "XOPStandardHeaders.h"

PhotometricsCamera::PhotometricsCamera(const std::string& cameraName) :
	_identifier(cameraName)
{
	int err = pl_cam_open(const_cast<char*>(cameraName.c_str()), &_pvcamHandle, OPEN_EXCLUSIVE);
	if (!err)
		throw std::runtime_error("failed to open photometrics camere");
}

PhotometricsCamera::~PhotometricsCamera() {
	pl_cam_close(_pvcamHandle);
}

std::string PhotometricsCamera::getIdentifierStr() const {
	return _identifier;
}

bool PhotometricsCamera::setExposureTime(const double exposureTime) {
	// ensure camera is set to accept exposure time in milliseconds
	int exposureTimeResolution = EXP_RES_ONE_MILLISEC;
	int err = pl_set_param(_pvcamHandle, PARAM_EXP_RES_INDEX, &exposureTimeResolution);
	if (err == 0) {
		return false;
	}

	double minExposureTime;
	err = pl_get_param(_pvcamHandle, PARAM_EXP_MIN_TIME, ATTR_CURRENT, &minExposureTime);
	if (err == 0) {
		return false;
	}

	if (exposureTime < minExposureTime) {
		return false;
	}

	_requestedExposureTime = exposureTime;
	return true;
}

bool PhotometricsCamera::setEMGain(const double emGain) {
	int result;
	if (emGain > 0) {
		int multFactor = emGain;
		int readoutPort = READOUT_PORT_MULT_GAIN;
		result = pl_set_param(_pvcamHandle, PARAM_READOUT_PORT, &readoutPort);
		if (!result)
			return false;
		result = pl_set_param(_pvcamHandle, PARAM_GAIN_MULT_FACTOR, &multFactor);
		if (!result)
			return false;
	} else {
		int readoutPort = READOUT_PORT_NORMAL;
		result = pl_set_param(_pvcamHandle, PARAM_READOUT_PORT, &readoutPort);
		if (!result)
			return false;
	}
	return true;
}

bool PhotometricsCamera::setTemperature(const double temperature) {
	int scaledSetPoint = temperature * 1000.0;
	int err = pl_set_param(_pvcamHandle, PARAM_TEMP_SETPOINT, &scaledSetPoint);
	if (err == 0) {
		return false;
	}
	else {
		return true;
	}
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
	int nRows, nCols;
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
		// handle me
	}

	std::uint32_t requiredMemorySpace;
	err = pl_exp_setup_seq(_pvcamHandle, nImages, 1, &region, TIMED_MODE, scaledExposureTime, reinterpret_cast<uns32_ptr>(&requiredMemorySpace));
	if (err == 0) {
		// handle me
	}
	if (requiredMemorySpace != (chipSize.first * chipSize.second * nImages * 2)) {
		// something is wrong with the requested amount of storage
		// possibly the camera is not set up to use uint16
		// handle me
	}

	std::vector<uint16_t> images(chipSize.first * chipSize.second * nImages);

	// start the acquisition
	err = pl_exp_start_seq(_pvcamHandle, reinterpret_cast<void*>(images.data()));
	if (err == 0) {
		// handle me
	}

	// wait until the camera has finished recording
	for ( ; ; ) {
		std::int16_t status;
		std::uint32_t nBytesRecorded;
		err = pl_exp_check_status(_pvcamHandle, &status, reinterpret_cast<uns32_ptr>(&nBytesRecorded));
		if (err == 0) {
			// handle me
		}

		if (status == READOUT_COMPLETE) {
			break;
		}

		// does the user wish to abort the exposure?
		int userAbort = CheckAbort(0);
		if (userAbort == -1) {
			err = pl_exp_abort(_pvcamHandle, CCS_NO_CHANGE);
			if (err == 0) {
				// handle me
			}
			break;
		}
	}

	return images;
}
