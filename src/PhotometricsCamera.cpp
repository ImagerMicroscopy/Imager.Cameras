
#include "SCConfigure.h"

#ifdef WITH_PHOTOMETRICS

#include "PhotometricsCamera.h"

#include <chrono>
#include <algorithm>
#include <thread>

#include "Utils.h"

#ifdef WITH_IGOR
	#include "XOPStandardHeaders.h"
#endif

std::string PhotometricsCamera::SpeedEntry::_generateDescriptor() const {
	int readoutRate = 1.0 / (_pixelTime * 1.0e-9) / 1.0e6;
	char buf[128];
	snprintf(buf, sizeof(buf), "%d MHz (%d bit)", readoutRate, _bitDepth);
	return std::string(buf);
}

PhotometricsCamera::PhotometricsCamera(const std::string& cameraName) :
	_installedCallbackFunction(false),
	_binningFactor(1),
	_crop(0, 0)
{
	int err = pl_cam_open(const_cast<char*>(cameraName.c_str()), &_pvcamHandle, OPEN_EXCLUSIVE);
	if (!err)
		throw std::runtime_error(getPVCAMErrorMessage());

	char prodName[MAX_PRODUCT_NAME_LEN + 1];
	char serNum[MAX_ALPHA_SER_NUM_LEN + 1];
	_fillCameraTextParameter(PARAM_PRODUCT_NAME, prodName);
	_fillCameraTextParameter(PARAM_HEAD_SER_NUM_ALPHA, serNum);
	_identifier = prodName;
	_identifier += "_";
	_identifier += serNum;

	_readoutPorts = _listReadoutPorts();

	_setDefaults();
}

PhotometricsCamera::~PhotometricsCamera() {
	pl_cam_close(_pvcamHandle);
}

std::string PhotometricsCamera::getIdentifierStr() const {
	return _identifier;
}

double PhotometricsCamera::getFrameRate() const {
	double exposureTime = _getExposureTime();
	std::uint32_t readoutTimeus = _getCameraParameterCurrentValue<std::uint32_t>(PARAM_READOUT_TIME);
	return (1.0 / (exposureTime + static_cast<double>(readoutTimeus) / 1.0e6));
}

void PhotometricsCamera::_setDefaults() {
	// exposure times are expressed in microseconds
	_setCameraParameter<std::int32_t>(PARAM_EXP_RES_INDEX, EXP_RES_ONE_MICROSEC);

	_setImageCrop(_getSensorSize());
	_setBinningFactor(1);
	_setTrigggerMode(EXT_TRIG_INTERNAL | EXPOSE_OUT_ROLLING_SHUTTER);
	_setExposureTime(50.0e-3);
}

std::vector<CameraProperty> PhotometricsCamera::_derivedGetCameraProperties() {
	std::vector<CameraProperty> properties;
	properties = GetStandardProperties(_getExposureTime(), _getImageCrop(), StandardCroppingOptions(_getSensorSize()), _getBinningFactor(), { 1, 2});

	properties.push_back(_getSetReadoutPort(GetProperty, std::string()));
	properties.push_back(_getSetReadoutSpeed(GetProperty, std::string()));
	properties.push_back(_getSetGain(GetProperty, std::string()));
	properties.push_back(_getSetTriggerMode(GetProperty, std::string()));

	return properties;
}

void PhotometricsCamera::_derivedSetCameraProperties(const std::vector<CameraProperty>& properties) {
	std::vector<CameraProperty> propsCopy(properties);

	std::optional<double> exposureTime;
	std::optional<std::pair<int, int>> cropping;
	std::optional<int> binningFactor;
	std::tie(exposureTime, cropping, binningFactor) = DecodeAndRemoveStandardProperties(propsCopy);

	if (cropping.has_value()) {
		_setImageCrop(cropping.value());
	}
	if (binningFactor.has_value()) {
		_setBinningFactor(binningFactor.value());
	}
	if (exposureTime.has_value()) {
		_setExposureTime(exposureTime.value());
	}

	for (const auto& prop : propsCopy) {
		switch (prop.getPropertyCode()) {
			case PropReadoutPort:
				_getSetReadoutPort(SetProperty, prop.getCurrentOption());
				break;
			case PropReadoutSpeed:
				_getSetReadoutSpeed(SetProperty, prop.getCurrentOption());
				break;
			case PropGain:
				_getSetGain(SetProperty, prop.getCurrentOption());
				break;
			case PropTriggerMode:
				_getSetTriggerMode(SetProperty, prop.getCurrentOption());
				break;
			default:
				throw std::runtime_error("setting unrecognized option");
		}
	}
}

CameraProperty PhotometricsCamera::_getSetReadoutPort(GetOrSetProperty getOrSet, const std::string& port) {
	if (getOrSet == SetProperty) {
		auto it = std::find_if(_readoutPorts.cbegin(), _readoutPorts.cend(), [&](const ReadoutPort& p) {
			return (p.name() == port);
		});
		if (it == _readoutPorts.cend()) {
			throw std::runtime_error("unknown readout port in PhotometricsCamera::_getSetReadoutPort()");
		}
		_setCameraParameter<std::int32_t>(PARAM_READOUT_PORT, it->index());
	}

	std::int32_t currentIndex = _getCameraParameterCurrentValue<std::int32_t>(PARAM_READOUT_PORT);
	auto it = std::find_if(_readoutPorts.cbegin(), _readoutPorts.cend(), [=](const ReadoutPort& p) {
		return (p.index() == currentIndex);
	});
	if (it == _readoutPorts.cend()) {
		throw std::runtime_error("unknown readout port index in PhotometricsCamera::_getSetReadoutPort()");
	}

	const std::string currentPortName = it->name();
	std::vector<std::string> portNames;
	for (const auto& p : _readoutPorts) {
		portNames.emplace_back(p.name());
	}
	CameraProperty prop(PropReadoutPort, "Readout port");
	prop.setDiscrete(currentPortName, portNames);
	return prop;
}

CameraProperty PhotometricsCamera::_getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string& descriptor) {
	const auto[currentReadoutPort, currentReadoutSpeed, currentGain] = _getCurrentReadoutSettings();
	const std::vector<SpeedEntry>& speedTable = currentReadoutPort.speedTable();

	if (getOrSet == SetProperty) {
		auto it = std::find_if(speedTable.cbegin(), speedTable.cend(), [&](const SpeedEntry& s) {
			return (s.descriptor() == descriptor);
		});
		if (it == speedTable.cend()) {
			throw std::runtime_error("unknown speed entry in PhotometricsCamera::_getSetReadoutSpeed()");
		}
		_setCameraParameter<std::int32_t>(PARAM_SPDTAB_INDEX, it->index());
	}

	const auto[updatedReadoutPort, updatedReadoutSpeed, updatedGain] = _getCurrentReadoutSettings();


	const std::string& currentDescriptor = updatedReadoutSpeed.descriptor();
	std::vector<std::string> speedDescriptors;
	for (const auto& s : speedTable) {
		speedDescriptors.emplace_back(s.descriptor());
	}
	CameraProperty prop(PropReadoutSpeed, "Readout speed");
	prop.setDiscrete(currentDescriptor, speedDescriptors);
	return prop;
}

CameraProperty PhotometricsCamera::_getSetGain(GetOrSetProperty getOrSet, const std::string & descriptor) {
	const auto[currentReadoutPort, currentReadoutSpeed, currentGain] = _getCurrentReadoutSettings();
	const std::vector<Gain>& gains = currentReadoutSpeed.gains();

	if (getOrSet == SetProperty) {
		auto it = std::find_if(gains.cbegin(), gains.cend(), [&](const auto& g) {
			return (g.descriptor() == descriptor);
		});
		if (it == gains.cend()) {
			throw std::runtime_error("unknown gain in PhotometricsCamera::_getSetGain()");
		}
		_setCameraParameter<std::int16_t>(PARAM_GAIN_INDEX, it->index());
	}

	const auto[updatedReadoutPort, updatedReadoutSpeed, updatedGain] = _getCurrentReadoutSettings();
	const std::string& currentDescriptor = updatedGain.descriptor();
	std::vector<std::string> gainDescriptors;
	for (const auto& g : updatedReadoutSpeed.gains()) {
		gainDescriptors.emplace_back(g.descriptor());
	}
	CameraProperty prop(PropGain, "Gain");
	prop.setDiscrete(currentDescriptor, gainDescriptors);
	return prop;
}

CameraProperty PhotometricsCamera::_getSetTriggerMode(GetOrSetProperty getOrSet, const std::string & mode) {
	const std::vector<std::pair<std::string, int>> modes = _getTriggerModes();
	if (getOrSet == SetProperty) {
		auto it = std::find_if(modes.cbegin(), modes.cend(), [&](const auto& m) {
			return (m.first == mode);
		});
		if (it == modes.cend()) {
			throw std::runtime_error("unknown mode to be set in PhotometricsCamera::_getSetTriggerMode()");
		}
		_triggerMode = it->second;
	}

	auto it = std::find_if(modes.cbegin(), modes.cend(), [&](const auto& m) {
		return (m.second == _triggerMode);
	});
	if (it == modes.cend()) {
		throw std::runtime_error("unknown trigger mode index in PhotometricsCamera::_getSetTriggerMode()");
	}

	const std::string currentMode = it->first;
	std::vector<std::string> modeNames;
	for (const auto& m : modes) {
		modeNames.emplace_back(m.first);
	}
	CameraProperty prop(PropTriggerMode, "Trigger Mode");
	prop.setDiscrete(currentMode, modeNames);
	return prop;
}

std::pair<int, int> PhotometricsCamera::_getSizeOfRawImages() const {
	std::pair<int, int> cropped = _getImageCrop();
	cropped.first /= _getBinningFactor();
	cropped.second /= _getBinningFactor();
	return cropped;
}

std::pair<int, int> PhotometricsCamera::_getSensorSize() const {
	int nRows = _getCameraParameterCurrentValue<std::uint16_t>(PARAM_SER_SIZE);
	int nCols = _getCameraParameterCurrentValue<std::uint16_t>(PARAM_PAR_SIZE);
	return std::pair<int, int>(nRows, nCols);
}

double PhotometricsCamera::_getExposureTime() const {
	std::uint64_t expTime = _getCameraParameterCurrentValue<std::uint64_t>(PARAM_EXPOSURE_TIME);
	return (expTime / 1.0e6);
}

void PhotometricsCamera::_setExposureTime(const double exposureTime) {
	std::pair<std::uint64_t, std::uint64_t> limits = _getCameraParameterLimits<std::uint64_t>(PARAM_EXPOSURE_TIME);
	std::uint64_t clamped = clamp((std::uint64_t)(exposureTime * 1.0e6), limits.first, limits.second);
	rgn_type region = _getRegionForCurrentBinningAndCropping();
	std::uint32_t nBytesInImage = 0;
	int err = pl_exp_setup_cont(_pvcamHandle, 1, &region, _triggerMode, clamped, reinterpret_cast<uns32_ptr>(&nBytesInImage), CIRC_OVERWRITE);
	if (err != PV_OK) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}
}

void PhotometricsCamera::_updateCameraTimings() {
	std::uint64_t expTime = _getCameraParameterCurrentValue<std::uint64_t>(PARAM_EXPOSURE_TIME);
	rgn_type region = _getRegionForCurrentBinningAndCropping();
	std::uint32_t nBytesInImage = 0;
	int err = pl_exp_setup_cont(_pvcamHandle, 1, &region, _triggerMode, expTime, reinterpret_cast<uns32_ptr>(&nBytesInImage), CIRC_OVERWRITE);
	if (err != PV_OK) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}
}

void PhotometricsCamera::_setImageCrop(const std::pair<int, int>& crop) {
	_crop = crop;
	_updateCameraTimings();
}

std::pair<int, int> PhotometricsCamera::_getImageCrop() const {
	return _crop;
}

void PhotometricsCamera::_setBinningFactor(const int binningFactor) {
	_binningFactor = binningFactor;
	_updateCameraTimings();
}

int PhotometricsCamera::_getBinningFactor() const {
	return _binningFactor;
}

rgn_type PhotometricsCamera::_getRegionForCurrentBinningAndCropping() const {
	std::pair<int, int> sensorSize = this->_getSensorSize();
	rgn_type region = { 0 };
	region.s1 = (sensorSize.first - _crop.first) / 2;
	region.s2 = sensorSize.first - 1 - region.s1;
	region.p1 = (sensorSize.second - _crop.second) / 2;
	region.p2 = sensorSize.second - 1 - region.p1;
	region.sbin = _binningFactor;
	region.pbin = _binningFactor;
	return region;
}

std::string PhotometricsCamera::getPVCAMErrorMessage() {
	char buf[ERROR_MSG_LEN];
	pl_error_message(pl_error_code(), buf);
	return std::string(buf);
}

std::vector<std::pair<std::string, int>> PhotometricsCamera::_getTriggerModes() const {
	std::vector<std::pair<std::string, int>> modes;
	modes.push_back(std::make_pair("Internal", EXT_TRIG_INTERNAL | EXPOSE_OUT_ROLLING_SHUTTER));
	modes.push_back(std::make_pair("External", EXT_TRIG_EDGE_RISING | EXPOSE_OUT_ROLLING_SHUTTER));
	modes.push_back(std::make_pair("External Start", EXT_TRIG_TRIG_FIRST | EXPOSE_OUT_ROLLING_SHUTTER));
	modes.push_back(std::make_pair("External Exposure (bulb)", EXT_TRIG_LEVEL | EXPOSE_OUT_ROLLING_SHUTTER));
	return modes;
}

void PhotometricsCamera::_derivedStartAsyncAcquisition() {
	int err = 0;
	std::uint64_t exposureTime = _getCameraParameterCurrentValue<std::uint64_t>(PARAM_EXPOSURE_TIME);
	std::uint32_t nBytesInImage;
	const int nImagesInBuffer = 10;

	// clear queue that signals new image is available
	for ( ; ; ) {
		int dummy = 0;
		bool haveValue = _pvcamCallbackQueue.try_dequeue(dummy);
		if (!haveValue) {
			break;
		}
	}

	if (!_installedCallbackFunction) {
		err = pl_cam_register_callback_ex3(_pvcamHandle, PL_CALLBACK_EOF, &_pvcamCallbackFunction, (void*)(&_pvcamCallbackQueue));
		if (err != PV_OK) {
			throw std::runtime_error("error installing pvcam callback");
		}
		_installedCallbackFunction = true;
	}

	std::pair<int, int> sensorSize = this->_getSensorSize();
	rgn_type region = { 0 };
	region.s1 = (sensorSize.first - _crop.first) / 2;
	region.s2 = sensorSize.first - 1 - region.s1;
	region.p1 = (sensorSize.second - _crop.second) / 2;
	region.p2 = sensorSize.second - 1 - region.p1;
	region.sbin = _binningFactor;
	region.pbin = _binningFactor;


	err = pl_exp_setup_cont(_pvcamHandle, 1, &region, _triggerMode, exposureTime, reinterpret_cast<uns32_ptr>(&nBytesInImage), CIRC_OVERWRITE);
	if (err != PV_OK) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}
	_asyncBuffer.resize(nBytesInImage * nImagesInBuffer);

	err = pl_exp_start_cont(_pvcamHandle, _asyncBuffer.data(), _asyncBuffer.size() * sizeof(std::uint16_t));
	if (err != PV_OK) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}
}

void PhotometricsCamera::_derivedAbortAsyncAcquisition() {
	pl_exp_stop_cont(_pvcamHandle, CCS_CLEAR);
}

bool PhotometricsCamera::_derivedNewAsyncAcquisitionImageAvailable() {
	int dummy = 0;
	return _pvcamCallbackQueue.wait_dequeue_timed(dummy, std::chrono::milliseconds(100));
}

void PhotometricsCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	uint16_t* address = nullptr;
	int err = pl_exp_get_oldest_frame(_pvcamHandle, reinterpret_cast<void**>(&address));
	if (err != PV_OK) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}
	memcpy(bufferForThisImage, address, nBytes);
	err = pl_exp_unlock_oldest_frame(_pvcamHandle);
	if (err != PV_OK) {
		throw std::runtime_error(getPVCAMErrorMessage());
	}
}

void PhotometricsCamera::_pvcamCallbackFunction(FRAME_INFO* infoPtr, void* contextPtr) {
	moodycamel::BlockingReaderWriterQueue<int>* queuePtr = (moodycamel::BlockingReaderWriterQueue<int>*)(contextPtr);
	queuePtr->enqueue(0);
}

std::vector<PhotometricsCamera::ReadoutPort> PhotometricsCamera::_listReadoutPorts() {
	std::vector<ReadoutPort> readoutPorts;
	std::vector<std::pair<std::int32_t, std::string>> portEnums = _getCameraEnumParameters(PARAM_READOUT_PORT);
	for (const auto& p : portEnums) {
		int portIndex = p.first;
		const std::string& portName = p.second;
		std::vector<SpeedEntry> speedEntries;
		_setCameraParameter<std::int32_t>(PARAM_READOUT_PORT, portIndex);
		std::uint32_t nSpeeds = _getCameraParameterCount(PARAM_SPDTAB_INDEX);
		for (std::uint32_t i = 0; i < nSpeeds; i += 1) {
			_setCameraParameter<std::int16_t>(PARAM_SPDTAB_INDEX, i);
			std::uint16_t pixelTime = _getCameraParameterCurrentValue<std::uint16_t>(PARAM_PIX_TIME);
			std::int16_t bitDepth = _getCameraParameterCurrentValue<std::uint16_t>(PARAM_BIT_DEPTH);
			std::vector<Gain> gains;
			std::pair<std::int16_t, std::int16_t> gainLimits = _getCameraParameterLimits<std::int16_t>(PARAM_GAIN_INDEX);
			for (std::uint16_t gainIdx = gainLimits.first; gainIdx <= gainLimits.second; gainIdx += 1) {
				_setCameraParameter<std::int16_t>(PARAM_GAIN_INDEX, gainIdx);
				char gainName[MAX_GAIN_NAME_LEN + 1];
				_fillCameraTextParameter(PARAM_GAIN_NAME, gainName);
				gains.emplace_back(gainIdx, gainName);
			}
			speedEntries.emplace_back(i, pixelTime, bitDepth, gains);
		}
		readoutPorts.emplace_back(portName, portIndex, speedEntries);
	}
	return readoutPorts;
}

std::tuple<PhotometricsCamera::ReadoutPort, PhotometricsCamera::SpeedEntry, PhotometricsCamera::Gain> PhotometricsCamera::_getCurrentReadoutSettings() const {
	std::int32_t currentReadoutPortIndex = _getCameraParameterCurrentValue<std::int32_t>(PARAM_READOUT_PORT);
	std::int16_t currentSpeedTableIndex = _getCameraParameterCurrentValue<std::int16_t>(PARAM_SPDTAB_INDEX);
	std::int16_t currentGainIndex = _getCameraParameterCurrentValue<std::int16_t>(PARAM_GAIN_INDEX);

	auto portIt = std::find_if(_readoutPorts.cbegin(), _readoutPorts.cend(), [=](const ReadoutPort& p) {
		return (p.index() == currentReadoutPortIndex);
	});
	if (portIt == _readoutPorts.cend()) {
		throw std::runtime_error("unknown readout port index in PhotometricsCamera::_getCurrentReadoutSettings()");
	}
	const ReadoutPort& currentReadoutPort = *portIt;
	
	const std::vector<SpeedEntry>& speedTable = currentReadoutPort.speedTable();
	auto speedIt = std::find_if(speedTable.cbegin(), speedTable.cend(), [&](const SpeedEntry& s) {
		return (s.index() == currentSpeedTableIndex);
	});
	if (speedIt == speedTable.cend()) {
		throw std::runtime_error("unknown speed entry in PhotometricsCamera::_getCurrentReadoutSettings()");
	}
	const SpeedEntry& currentSpeedEntry = *speedIt;

	const std::vector<Gain>& gains = currentSpeedEntry.gains();
	auto gainIt = std::find_if(gains.cbegin(), gains.cend(), [&](const Gain& g) {
		return (g.index() == currentGainIndex);
	});
	if (gainIt == gains.cend()) {
		throw std::runtime_error("unknown gain entry in PhotometricsCamera::_getCurrentReadoutSettings()");
	}
	const Gain& currentGain = *gainIt;

	return std::make_tuple(currentReadoutPort, currentSpeedEntry, currentGain);
}

std::vector<std::pair<std::int32_t, std::string>> PhotometricsCamera::_getCameraEnumParameters(int paramID) const {
	std::vector<std::pair<std::int32_t, std::string>> params;
	std::uint32_t count = _getCameraParameterCount(paramID);
	for (std::uint32_t i = 0; i < count; ++i) {
		std::int32_t value = 0;
		char desc[128] = { 0 };
		rs_bool result = pl_get_enum_param(_pvcamHandle, paramID, i, &value, desc, 127);
		if (result != PV_OK) {
			throw std::runtime_error("invalid result in PhotometricsCamera::_getCameraEnumParameters()");
		}
		params.emplace_back(value, desc);
	}
	return params;
}

#endif
