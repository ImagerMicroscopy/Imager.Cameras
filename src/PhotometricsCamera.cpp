
#include "PhotometricsCamera.h"

PhotometricsCamera::PhotometricsCamera(const std::string& cameraName) {
	int err = pl_cam_open(cameraName.c_str(), &_pvcamHandle, OPEN_EXCLUSIVE);
	if (!err)
		throw std::runtime_error("failed to open photometrics camere");
}

PhotometricsCamera::~PhotometricsCamera() {
}

bool PhotometricsCamera::setExposureTime(const double exposureTime) {
	// ensure camera is set to accept exposure time in milliseconds
	int exposureTimeResolution = EXP_RES_ONE_MILLISEC;
	int err = pl_set_param(camHandle, PARAM_EXP_RES_INDEX, &exposureTimeResolution);
	if (err == 0) {
		return false;
	}

	double minExposureTime;
	err = pl_get_param(camHandle, PARAM_EXP_MIN_TIME, ATTR_CURRENT, &minExposureTime);
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
		result = pl_set_param(camHandle, PARAM_GAIN_MULT_FACTOR, &multFactor);
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
	err = pl_set_param(camHandle, PARAM_TEMP_SETPOINT, &scaledSetPoint);
	if (err == 0) {
		return false;
	}
	else {
		return true;
	}
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
	err = pl_cam_get_diags(camHandle);
	if (err == 0) {
		// handle me
	}

	std::uint64_t requiredMemorySpace;
	err = pl_exp_setup_seq(_pvcamHandle, nImages, 1, &region, TIMED_MODE, scaledExposureTime, &requiredMemorySpace);
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
	err = pl_exp_start_seq(_pvcamHandle, reinterpret_cast<void*>(images->data()));
	if (err == 0) {
		// handle me
	}

	// wait until the camera has finished recording
	for ( ; ; ) {
		std::int16_t status;
		std::uint32_t nBytesRecorded;
		err = pl_exp_check_status(camHandle, &status, &nBytesRecorded);
		if (err == 0) {
			// handle me
		}

		if (status == READOUT_COMPLETE) {
			break;
		}

		// does the user wish to abort the exposure?
		userAbort = CheckAbort(0);
		if (userAbort == -1) {
			err = pl_exp_abort(camHandle, CCS_NO_CHANGE);
			if (err == 0) {
				// handle me
			}
			break;
		}
	}

	return images;
}

	return err;
}
