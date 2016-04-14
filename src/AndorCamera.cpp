
#include "SCConfigure.h"

#ifdef WITH_ANDOR

#include <chrono>
#include <algorithm>
#include <thread>

#include "AndorCamera.h"
#include "Exceptions.h"
#include "Andor/ATMCD32D.H"

#include "XOPStandardHeaders.h"

AndorCamera::AndorCamera() :
	_temperatureSetpoint(0)
{
	int err = Initialize(nullptr);
	if (err != DRV_SUCCESS)
		throw std::runtime_error("No Andor camera");

	_setDefaults();
}

AndorCamera::~AndorCamera() {
	SetShutter(1, 2, 100, 100);		// close shutter

	setCoolerOn(false);

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

bool AndorCamera::setCoolerOn(const bool on) {
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
	int tempSetpoint = std::round(temperature);

	int result = GetTemperatureRange(&minTemp, &maxTemp);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	tempSetpoint = std::min(std::max(tempSetpoint, minTemp), maxTemp);

	result = SetTemperature(tempSetpoint);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	_temperatureSetpoint = tempSetpoint;

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
	if ((result != DRV_SUCCESS) && (result != DRV_TEMP_STABILIZED) && (result != DRV_TEMP_NOT_REACHED)
		&& (result != DRV_TEMP_DRIFT) && (result != DRV_TEMP_NOT_STABILIZED))
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

	setCoolerOn(false);

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
		case DRV_TEMPERATURE_NOT_STABILIZED:
			message = "temperature not stabilized";
			break;
		case DRV_TEMP_OFF:
			message = "temperature is OFF";
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

void AndorCamera::_derivedStartAsyncAcquisition() {
	int result;
	_numberOfImagesAcquired = 0;

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

	// set run till abort
	result = SetAcquisitionMode(5);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = StartAcquisition();
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
}
void AndorCamera::_derivedAbortAsyncAcquisition() {
	AbortAcquisition();
}
bool AndorCamera::_derivedNewAsyncAcquisitionImageAvailable() {
	long nImagesAcquired;
	int result = GetTotalNumberImagesAcquired(&nImagesAcquired);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	return (nImagesAcquired != _numberOfImagesAcquired);
}
void AndorCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	int result;
	long firstImageIndex, lastImageIndex;
	long validFirstIndex, validLastIndex;
	result = GetNumberNewImages(&firstImageIndex, &lastImageIndex);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = GetImages16(firstImageIndex, firstImageIndex, bufferForThisImage, nBytes / sizeof(std::uint16_t), &validFirstIndex, &validLastIndex);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	if ((validFirstIndex != firstImageIndex) || (validLastIndex != firstImageIndex)) {
		throw std::runtime_error("valid index different from requested index");
	}
	_numberOfImagesAcquired += 1;
}

#endif
