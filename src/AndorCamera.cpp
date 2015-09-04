
#include "SCConfigure.h"

#ifdef WITH_ANDOR

#include "AndorCamera.h"
#include "Andor/ATMCD32D.H"

#include "XOPStandardHeaders.h"

AndorCamera::AndorCamera() {
	int err = Initialize(nullptr);
	if (err != DRV_SUCCESS)
		throw std::runtime_error("No Andor camera");

	_setDefaults();
}

AndorCamera::~AndorCamera() {
	SetShutter(1, 2, 100, 100);		// close shutter

	ShutDown();
}

std::string AndorCamera::getIdentifierStr() const {
	char buf[MAX_PATH];
	GetHeadModel(buf);
	return std::string(buf);
}

bool AndorCamera::setExposureTime(const double exposureTime) {
	int result = SetExposureTime(static_cast<float>(exposureTime));
	if (result != DRV_SUCCESS) {
		if (result != DRV_P1INVALID) {
		throw std::runtime_error(_andorErrorCodeToMessage(result));
		} else {
			return false;
		}
	}
	return true;
}

bool AndorCamera::setEMGain(const double emGain) {
	int minGain, maxGain;
	int result = GetEMGainRange(&minGain, &maxGain);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	if ((emGain < minGain) || (emGain > maxGain))
		return false;

	result = SetEMCCDGain(std::round(emGain));
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	return true;
}

bool AndorCamera::setCoolerOnOff(const bool on) {
	int result;
	if (on) {
		result = CoolerON();
		if (result != DRV_SUCCESS)
			throw std::runtime_error(_andorErrorCodeToMessage(result));
		_coolerOn = true;
	} else {
		result = CoolerOFF();
		if (result != DRV_SUCCESS)
			throw std::runtime_error(_andorErrorCodeToMessage(result));
		_coolerOn = false;
	}

	return true;
}

bool AndorCamera::setTemperature(const double temperature) {
	int minTemp, maxTemp;
	int result = GetTemperatureRange(&minTemp, &maxTemp);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	if ((temperature < minTemp) || (temperature > maxTemp))
		return false;

	result = SetTemperature(std::round(temperature));
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	return true;
}

double AndorCamera::getExposureTime() const {
	float exposureTime, accumulate, kinetic;
	int result = GetAcquisitionTimings(&exposureTime, &accumulate, &kinetic);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	return exposureTime;
}

double AndorCamera::getEMGain() const {
	int gain;
	int result = GetEMCCDGain(&gain);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	return static_cast<double>(gain);
}

double AndorCamera::getTemperature() const {
	int temperature;
	int result = GetTemperature(&temperature);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	return static_cast<double>(temperature);
}

std::pair<int, int> AndorCamera::getSensorSize() const {
	int nRows, nCols;
	int result = GetDetector(&nCols, &nRows);	// TODO: incorrect for cropped sensors?
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	return std::pair<int, int>(nRows, nCols);
}

std::vector<uint16_t> AndorCamera::acquireImages(const int nImages) {
	int result = SetNumberKinetics(nImages);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetKineticCycleTime(0.0);			// grab frames as fast as they come
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	// Check if the camera is ready to measure
	int status;
	result = GetStatus(&status);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	if (status != DRV_IDLE)
		throw std::runtime_error(_andorErrorCodeToMessage(status));

	// Start acquiring images
	result = StartAcquisition();
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	// Wait until all images have been acquired
	for (;;) {
		result = GetStatus(&status);
		if (result != DRV_SUCCESS)
			throw std::runtime_error(_andorErrorCodeToMessage(result));
		if (status == DRV_IDLE)			// acquisition finished
			break;
		if (status != DRV_ACQUIRING)	// error
			throw std::runtime_error(_andorErrorCodeToMessage(status));

		if (SpinProcess())
			throw int(USER_ABORT);
	}

	// allocate storage to hold the image
	std::pair<int, int> sensorSize = getSensorSize();
	int nElements = sensorSize.first * sensorSize.second * nImages;
	std::vector<std::uint16_t> images(nElements);

	result = GetAcquiredData16(images.data(), nElements);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	return images;
}

void AndorCamera::_setDefaults() {
	int result;

	result = SetReadMode(4);					// set image mode
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	std::pair<int, int> sensorSize = getSensorSize();	// use the full chip, no binning
	result = SetImage(1, 1, 1, sensorSize.second, 1, sensorSize.first);

	result = SetAcquisitionMode(3);				// set kinetics mode
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetFrameTransferMode(1);			// enable frame transfer
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetKineticCycleTime(0.0);			// grab frames as fast as they come
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetTriggerMode(0);					// internal trigger mode
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetEMGainMode(0);					// EM Gain is controlled by DAC settings in the range 0-255
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetOutputAmplifier(0);				// use EMCCD gain register
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	setCoolerOnOff(false);

	result = SetShutter(1, 1, 100, 100);		// open shutter
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	setExposureTime(50.0e-3);
}

void AndorCamera::_selectFastestRecommendedReadoutSpeed() {
	int index;
	float speed;
	int result = GetFastestRecommendedVSSpeed(&index, &speed);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetVSSpeed(index);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
}

std::string AndorCamera::_andorErrorCodeToMessage(int errorCode) const {
	std::string message;
	switch (errorCode) {
		case DRV_SUCCESS:
			message = "no error";
			break;
		case DRV_VXDNOTINSTALLED:
			message = "VxD not loaded";
			break;
		case DRV_INIERROR:
			message = "unable to load DETECTOR.INI";
			break;
		case DRV_COFERROR:
			message = "unable to load *.COF";
			break;
		case DRV_FLEXERROR:
			message = "unable to load *.RBF";
			break;
		case DRV_ERROR_ACK:
			message = "unable to communicate with card";
			break;
		case DRV_ERROR_FILELOAD:
			message = "unable to load *.COF or *.RBF files";
			break;
		case DRV_ERROR_PAGELOCK:
			message = "unable to acquire lock on requested memory";
			break;
		case DRV_USBERROR:
			message = "unable to detect USB device or not USB2.0";
			break;
		case DRV_ERROR_NOCAMERA:
			message = "no camera found";
			break;
		case DRV_NOT_INITIALIZED:
			message = "system not initialized";
			break;
		case DRV_ACQUIRING:
			message = "acquisition in progress";
			break;
		case DRV_P1INVALID:
			message = "parameter 1 invalid";	// context-specific
			break;
		case DRV_ACQUISITION_ERRORS:
			message = "acquisition settings invalid";
			break;
		case DRV_INVALID_FILTER:
			message = "filter not available for current acquisition";
			break;
		case DRV_BINNING_ERROR:
			message = "range not multiple of current binning";
			break;
		case DRV_SPOOLSETUPERROR:
			message = "error with spool settings";
			break;
		default:
		{
				   char buf[128];
				   sprintf_s(buf, "andor error code %d", errorCode);
				   message = buf;
				   break;
		}
	}

	return message;
}

#endif
