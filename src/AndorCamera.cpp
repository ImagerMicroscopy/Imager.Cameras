
#include "SCConfigure.h"

#ifdef WITH_ANDOR

#include <chrono>
#include <algorithm>
#include <thread>

#include "AndorCamera.h"
#include "Exceptions.h"
#include "Utils.h"

#ifdef WITH_IGOR
#include "XOPStandardHeaders.h"
#endif
AndorCamera::AndorCamera() :
	_horizontalReadoutSpeedIndex(0),
	_verticalReadoutSpeedIndex(0),
	_temperatureSetpoint(0),
	_desiredBinningFactor(1)
{
	int err = Initialize(nullptr);
	if (err != DRV_SUCCESS)
		throw std::runtime_error("No Andor camera");

	_setDefaults();
	_desiredCropSize = _getSensorSize();
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

std::pair<int, int> AndorCamera::getActualImageSize() const {
	auto size = _desiredCropSize;
	size.first /= _desiredBinningFactor;
	size.second /= _desiredBinningFactor;
	return size;
}

double AndorCamera::getFrameRate() const {
	float exposureTime, accumulate, kinetic;
	int result = GetAcquisitionTimings(&exposureTime, &accumulate, &kinetic);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	return (1.0 / kinetic);
}

std::vector<CameraProperty> AndorCamera::_derivedGetCameraProperties() {
	std::vector<CameraProperty> properties = GetStandardProperties(_getExposureTime(), _desiredCropSize, StandardCroppingOptions(_getSensorSize()),
		_desiredBinningFactor, StandardBinningOptions());

	if (_hasEMGain()) {
		properties.push_back(_getSetEMGain(GetProperty, 0.0));
	}
	if (_hasFrameTransferMode()) {
		properties.push_back(_getSetFrameTransferMode(GetProperty, std::string()));
	}
	properties.push_back(_getSetHorizontalReadoutSpeeds(GetProperty, std::string()));
	properties.push_back(_getSetCoolerOn(GetProperty, std::string()));
	properties.push_back(_getSetTemperatureSetPoint(GetProperty, 0.0));
	return properties;
}

void AndorCamera::_derivedSetCameraProperties(const std::vector<CameraProperty>& properties) {
	std::vector<CameraProperty> propsCopy(properties);
	double exposureTime = 0;
	std::pair<int, int> cropping(512, 512);
	int binningFactor = 1;
	std::tie(exposureTime, cropping, binningFactor) = DecodeAndRemoveStandardProperties(propsCopy);

	_setCroppingAndBinning(cropping, binningFactor);
	_setExposureTime(exposureTime);

	for (const auto& prop : propsCopy) {
		switch (prop.getPropertyCode()) {
			case PropEMGain:
				_getSetEMGain(SetProperty, prop.getValue());
				break;
			case PropFrameTransferMode:
				_getSetFrameTransferMode(SetProperty, prop.getCurrentOption());
				break;
			case PropVerticalReadoutSpeed:
				_getSetVerticalReadoutSpeeds(SetProperty, prop.getCurrentOption());
				break;
			case PropHorizontalReadoutSpeed:
				_getSetHorizontalReadoutSpeeds(SetProperty, prop.getCurrentOption());
				break;
			case PropTemperatureSetPoint:
				_getSetTemperatureSetPoint(SetProperty, prop.getValue());
				break;
			case PropCoolerOn:
				_getSetCoolerOn(SetProperty, prop.getCurrentOption());
				break;
			default:
				throw std::runtime_error("setting unrecognized option");
		}
	}
}

std::pair<int, int> AndorCamera::_getSensorSize() const {
	int nRows, nCols;
	int result = GetDetector(&nCols, &nRows);	// TODO: incorrect for cropped sensors?
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	return std::pair<int, int>(nRows, nCols);
}

AndorCapabilities AndorCamera::_getCapabilities() const {
	AndorCapabilities caps;
	caps.ulSize = sizeof(caps);
	if (GetCapabilities(&caps) != DRV_SUCCESS) {
		throw std::runtime_error("can't query caps");
	}
	return caps;
}

bool AndorCamera::_hasFrameTransferMode() const {
	auto caps = _getCapabilities();
	return (caps.ulAcqModes & AC_ACQMODE_FRAMETRANSFER);
}

bool AndorCamera::_hasEMGain() const {
	auto caps = _getCapabilities();
	return (caps.ulEMGainCapability & 0x0F);
}

CameraProperty AndorCamera::_getSetFrameTransferMode(GetOrSetProperty getOrSet, const std::string & mode) {
	static const char* kFTOn = "On";
	static const char* kFTOff = "Off";
	if (getOrSet == SetProperty) {
		if (mode == kFTOn) {
			int result = SetFrameTransferMode(1);
			if (result == DRV_SUCCESS) {
				_frameTransferModeOn = true;
			}
		}
		else if (mode == kFTOff) {
			int result = SetFrameTransferMode(0);
			if (result == DRV_SUCCESS) {
				_frameTransferModeOn = false;
			}
		}
		else {
			throw std::runtime_error("unknown frame transfer mode specifier");
		}
	}

	CameraProperty prop(PropFrameTransferMode, "Frame transfer");
	const char* currentOption = (_frameTransferModeOn) ? kFTOn : kFTOff;
	prop.setDiscrete(currentOption, { kFTOff, kFTOn });
	return prop;
}

CameraProperty AndorCamera::_getSetEMGain(GetOrSetProperty getOrSet, const double setPoint) {
	if (getOrSet == SetProperty) {
		SetEMCCDGain(setPoint);
	}
	CameraProperty prop(PropEMGain, "EM Gain");
	int gain = 0;
	GetEMCCDGain(&gain);
	prop.setNumeric(gain);
	return prop;
}

CameraProperty AndorCamera::_getSetHorizontalReadoutSpeeds(GetOrSetProperty getOrSet, const std::string& mode) {
	static std::vector<std::string> availableSpeeds;
	if (availableSpeeds.empty()) {
		int nSpeeds = 0;
		int result = GetNumberHSSpeeds(0, 0, &nSpeeds);
		if (result != DRV_SUCCESS) {
			throw std::runtime_error("error getting number of horizontal readout speeds");
		}
		for (int i = 0; i < nSpeeds; i += 1) {
			float speed = 0.0f;
			int result = GetHSSpeed(0, 0, i, &speed);
			char buf[128];
			sprintf(buf, "%f MHz", speed);
			availableSpeeds.emplace_back(buf);
		}
	}

	if (getOrSet == SetProperty) {
		auto it = std::find(availableSpeeds.cbegin(), availableSpeeds.cend(), mode);
		if (it == availableSpeeds.cend()) throw std::runtime_error("couldn't find HS speed");
		int theIndex = it - availableSpeeds.cbegin();
		SetHSSpeed(0, theIndex);
		_horizontalReadoutSpeedIndex = theIndex;
	}
	CameraProperty prop(PropHorizontalReadoutSpeed, "Horizontal readout speed");
	prop.setDiscrete(availableSpeeds.at(_horizontalReadoutSpeedIndex), availableSpeeds);
	return prop;
}

CameraProperty AndorCamera::_getSetVerticalReadoutSpeeds(GetOrSetProperty getOrSet, const std::string& mode) {
	static std::vector<std::string> availableSpeeds;
	if (availableSpeeds.empty()) {
		int nSpeeds = 0;
		int result = GetNumberVSSpeeds(&nSpeeds);
		if (result != DRV_SUCCESS) {
			throw std::runtime_error("error getting number of horizontal readout speeds");
		}
		int maxIndex = 0;
		float dummy = 0.0f;
		result = GetFastestRecommendedVSSpeed(&maxIndex, &dummy);
		if (result != DRV_SUCCESS) throw std::runtime_error("error getting fastest V readout speed");
		for (int i = 0; i < maxIndex; i += 1) {
			float speed = 0.0f;
			int result = GetVSSpeed(i, &speed);
			char buf[128];
			sprintf(buf, "%f MHz", speed);
			availableSpeeds.emplace_back(buf);
		}
	}

	if (getOrSet == SetProperty) {
		auto it = std::find(availableSpeeds.cbegin(), availableSpeeds.cend(), mode);
		if (it == availableSpeeds.cend()) throw std::runtime_error("couldn't find VS speed");
		int theIndex = it - availableSpeeds.cbegin();
		int result = SetVSSpeed(theIndex);
		if (result != DRV_SUCCESS) throw std::runtime_error("error setting V readout speed");
		_verticalReadoutSpeedIndex = theIndex;
	}
	CameraProperty prop(PropVerticalReadoutSpeed, "Vertical readout speed");
	prop.setDiscrete(availableSpeeds.at(_verticalReadoutSpeedIndex), availableSpeeds);
	return prop;
}

CameraProperty AndorCamera::_getSetTemperatureSetPoint(GetOrSetProperty getOrSet, const double setPoint) {
	if (getOrSet == SetProperty) {
		int minTemp, maxTemp;
		int tempSetpoint = setPoint;
		GetTemperatureRange(&minTemp, &maxTemp);
		tempSetpoint = std::min(std::max(tempSetpoint, minTemp), maxTemp);
		int result = SetTemperature(tempSetpoint);
		if (result == DRV_SUCCESS) {
			_temperatureSetpoint = tempSetpoint;
		}
	}
	CameraProperty prop(PropTemperatureSetPoint, "Temperature setpoint");
	prop.setNumeric(_temperatureSetpoint);
	return prop;
}

CameraProperty AndorCamera::_getSetCoolerOn(GetOrSetProperty getOrSet, const std::string & mode) {
	static const char* kOn = "On";
	static const char* kOff = "Off";
	if (getOrSet == SetProperty) {
		if (mode == kOn) {
			CoolerON();
		}
		else if (mode == kOff) {
			CoolerOFF();
		}
		else {
			throw std::runtime_error("unknown cooler mode specifier");
		}
	}
	int coolerStatus = 0;
	IsCoolerOn(&coolerStatus);
	CameraProperty prop(PropCoolerOn, "Cooler");
	const char* currentOption = (coolerStatus) ? kOn : kOff;
	prop.setDiscrete(currentOption, { kOff, kOn });
	return prop;
}

void AndorCamera::_setExposureTime(const double exposureTime) {
	int result = SetExposureTime(static_cast<float>(exposureTime));
	if ((result != DRV_SUCCESS) && (result != DRV_P1INVALID)) {
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	}
}

double AndorCamera::_getExposureTime() const {
	float exposureTime, accumulate, kinetic;
	int result = GetAcquisitionTimings(&exposureTime, &accumulate, &kinetic);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	return exposureTime;
}

void AndorCamera::_setCroppingAndBinning(const std::pair<int, int>& crop, const int binningFactor) {
	std::pair<int, int> sensorSize = _getSensorSize();
	int offsetX = (sensorSize.first - crop.first) / 2;
	int offsetY = (sensorSize.second - crop.second) / 2;

	int result = SetImage(binningFactor, binningFactor, offsetX + 1, offsetX + crop.first, offsetY + 1, offsetY + crop.second);
	if (result != DRV_SUCCESS) {
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	}
}

void AndorCamera::_setDefaults() {
	int result;

	CoolerOFF();

	result = SetReadMode(4);					// set image mode
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	std::pair<int, int> sensorSize = _getSensorSize();	// use the full chip, no binning
	_desiredCropSize = sensorSize;
	_desiredBinningFactor = 1;
	result = SetImage(1, 1, 1, sensorSize.second, 1, sensorSize.first);

	result = SetAcquisitionMode(3);				// set kinetics mode
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetFrameTransferMode(0);			// disable frame transfer
	_frameTransferModeOn = false;

	result = SetKineticCycleTime(0.0);			// grab frames as fast as they come
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetTriggerMode(0);					// internal trigger mode
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetEMGainMode(0);					// EM Gain is controlled by DAC settings in the range 0-255
												//if (result != DRV_SUCCESS)
												//	throw std::runtime_error(_andorErrorCodeToMessage(result));

	result = SetOutputAmplifier(0);				// use EMCCD gain register
												//if (result != DRV_SUCCESS)
												//	throw std::runtime_error(_andorErrorCodeToMessage(result));

												// select fastest horizontal and vertical readout speeds
	CameraProperty prop = _getSetHorizontalReadoutSpeeds(GetProperty, std::string());
	_getSetHorizontalReadoutSpeeds(SetProperty, prop.getAvailableOptions().at(prop.getAvailableOptions().size() - 1));
	prop = _getSetVerticalReadoutSpeeds(GetProperty, std::string());
	_getSetVerticalReadoutSpeeds(SetProperty, prop.getAvailableOptions().at(prop.getAvailableOptions().size() - 1));

	int nGains;									// preamp gain
	result = GetNumberPreAmpGains(&nGains);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	if (nGains > 1) {
		float maxGain = -1.0; int maxGainIndex = 0;
		float gain = -1.0;
		for (int i = 0; i < nGains; i++) {
			GetPreAmpGain(i, &gain);
			if (gain > maxGain) {
				maxGain = gain;
				maxGainIndex = i;
			}
		}
		result = SetPreAmpGain(maxGainIndex);
		if (result != DRV_SUCCESS)
			throw std::runtime_error(_andorErrorCodeToMessage(result));
	}

	result = SetShutter(1, 1, 100, 100);		// open shutter
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));

	_setExposureTime(50.0e-3);
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

void AndorCamera::_derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) {
	auto imageSize = getActualImageSize();
	int nPixelsInImage = imageSize.first * imageSize.second;

	if (nBytes != (nPixelsInImage * sizeof(std::uint16_t))) {
		throw std::runtime_error("invalid number of bytes in _derivedAcquireSingleImage()");
	}

	int err = SetAcquisitionMode(1);	// single scan
	if (err != DRV_SUCCESS) throw std::runtime_error("Can't set single scan mode");

	err = StartAcquisition();
	if (err != DRV_SUCCESS) throw std::runtime_error("Can't start single acquisition");
	err = WaitForAcquisition();
	if (err != DRV_SUCCESS) throw std::runtime_error("Error waiting for single acquisition");
	err = GetMostRecentImage16(bufferForThisImage, nBytes / 2);
	if (err != DRV_SUCCESS) throw std::runtime_error("Error calling GetMostRecentImage16() in single acquisition");
}

void AndorCamera::_derivedStartAsyncAcquisition() {
	int result;
	_numberOfImagesDelivered = 0;

	result = SetAcquisitionMode(5);				// run till abort
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
	return (nImagesAcquired != _numberOfImagesDelivered);
}

bool AndorCamera::_waitForNewImageWithTimeout(int timeoutMillis) {
	if (_derivedNewAsyncAcquisitionImageAvailable()) {
		return true;
	}
	else {
		WaitForAcquisitionTimeOut(timeoutMillis);
		return _derivedNewAsyncAcquisitionImageAvailable();
	}
}

void AndorCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	int result = GetOldestImage16(bufferForThisImage, nBytes / 2);
	if (result != DRV_SUCCESS)
		throw std::runtime_error(_andorErrorCodeToMessage(result));
	_numberOfImagesDelivered += 1;
}

#endif
