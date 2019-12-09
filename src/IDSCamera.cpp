#include "SCConfigure.h"

#ifdef WITH_IDS

#include "IDSCamera.h"

IDSCamera::IDSCamera(HIDS camHandle) :
    _camHandle(camHandle)
  , _idOfMemoryWithOldestImage(-1)
  , _ptrToMemoryWithOldestImage(nullptr)
{
	_setDefaults();

	_desiredCropSize = _getSensorSize();
	_desiredBinningFactor = 1;
}

IDSCamera::~IDSCamera() {
    is_ExitCamera(_camHandle);
}

std::string IDSCamera::getIdentifierStr() const {
    CAMINFO camInfo;
    int err = is_GetCameraInfo(_camHandle, &camInfo);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to get IDS camera info");
    }
    std::string idStr("IDS_");
    idStr += std::string(camInfo.SerNo);
    return idStr;
}

std::pair<int, int> IDSCamera::getActualImageSize() const {
	auto size = _desiredCropSize;
	size.first /= _desiredBinningFactor;
	size.second /= _desiredBinningFactor;
	return size;
}

double IDSCamera::getFrameRate() const {
	double fps;
	is_SetFrameRate(_camHandle, IS_GET_FRAMERATE, &fps);
	return fps;
}

std::vector<CameraProperty> IDSCamera::_derivedGetCameraProperties() {
	std::vector<CameraProperty> properties;
	properties = GetStandardProperties(_getExposureTime(), _desiredCropSize, StandardCroppingOptions(_getSensorSize()), _desiredBinningFactor, StandardBinningOptions());

	if (_haveGainBoost()) {
		properties.push_back(_getSetGainBoost(GetProperty, std::string()));
		properties.push_back(_getSetHardwareGain(GetProperty, 0.0));
	}
	properties.push_back(_getSetPixelClock(GetProperty, std::string()));
	if (_haveHotPixelCorrection()) {
		properties.push_back(_getSetHotPixelCorrection(GetProperty, std::string()));
		properties.push_back(_getSetAdaptiveHotPixelCorrectionMode(GetProperty, std::string()));
		properties.push_back(_getSetAdaptiveHotPixelCorrectionSensitivity(GetProperty, 0.0));
	}

	return properties;
}

void IDSCamera::_derivedSetCameraProperties(const std::vector<CameraProperty>& properties) {
	std::vector<CameraProperty> propsCopy(properties);

	std::optional<double> exposureTime = 0;
	std::optional<std::pair<int, int>> cropping(std::pair<int, int>(512, 512));
	std::optional<int> binningFactor = 1;
	std::tie(exposureTime, cropping, binningFactor) = DecodeAndRemoveStandardProperties(propsCopy);

	if (cropping.has_value()) {
		_desiredCropSize = cropping.value();
	}
	if (binningFactor.has_value()) {
		_desiredBinningFactor = binningFactor.value();
	}
	if (exposureTime.has_value()) {
		_setExposureTime(exposureTime.value());
	}

	for (const auto& prop : propsCopy) {
		switch (prop.getPropertyCode()) {
			case PropGainBoost:
				_getSetGainBoost(SetProperty, prop.getCurrentOption());
				break;
			case PropHardwareGain:
				_getSetHardwareGain(SetProperty, prop.getValue());
				break;
			case PropPixelClock:
				_getSetPixelClock(SetProperty, prop.getCurrentOption());
				break;
			case PropHotPixelCorrection:
				_getSetHotPixelCorrection(SetProperty, prop.getCurrentOption());
				break;
			case PropAdaptiveHotPixelCorrectionMode:
				_getSetAdaptiveHotPixelCorrectionMode(SetProperty, prop.getCurrentOption());
				break;
			case PropAdaptiveHotPixelSensitivity:
				_getSetAdaptiveHotPixelCorrectionSensitivity(SetProperty, prop.getValue());
				break;
			default:
				throw std::runtime_error("setting unrecognized option");
		}
	}
}

std::pair<int, int> IDSCamera::_getSensorSize() const {
    IS_RECT rect;
    int err = is_AOI(_camHandle, IS_AOI_IMAGE_GET_AOI, &rect, sizeof(rect));
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to get AOI");
    }
    return std::pair<int, int>(rect.s32Width, rect.s32Height);
}

double IDSCamera::_getExposureTime() const {
	double exposureTimeMillis;
	int err = is_Exposure(_camHandle, IS_EXPOSURE_CMD_GET_EXPOSURE, &exposureTimeMillis, sizeof(exposureTimeMillis));
	if (err != IS_SUCCESS) {
		throw std::runtime_error("unable to get IDS exposure time");
	}
	return (exposureTimeMillis / 1000.0);
}

void IDSCamera::_setExposureTime(const double exposureTime) {
	double desiredFrameRate = 1.0 / exposureTime;
	double actualFrameRate = 0.0;
	is_SetFrameRate(_camHandle, desiredFrameRate, &actualFrameRate);
	double reqExposureTime = exposureTime * 1000.0; // If 0 is passed, the exposure time is set to the maximum value of 1/frame rate.
	is_Exposure(_camHandle, IS_EXPOSURE_CMD_SET_EXPOSURE, &reqExposureTime, sizeof(reqExposureTime));
}

CameraProperty IDSCamera::_getSetGainBoost(GetOrSetProperty getOrSet, const std::string & mode) {
	static const char* kOn = "On";
	static const char* kOff = "Off";
	if (getOrSet == SetProperty) {
		if (mode == kOn) {
			is_SetGainBoost(_camHandle, IS_SET_GAINBOOST_ON);
		} else if (mode == kOff) {
			is_SetGainBoost(_camHandle, IS_SET_GAINBOOST_OFF);
		} else {
			throw std::runtime_error("unknown mode setting gain boost");
		}
	}
	int result = is_SetGainBoost(_camHandle, IS_GET_GAINBOOST);
	const char* current = (result == IS_SET_GAINBOOST_ON) ? kOn : kOff;
	CameraProperty prop(PropGainBoost, "Gain boost");
	prop.setDiscrete(current, { kOff, kOn });
	return prop;
}

CameraProperty IDSCamera::_getSetHardwareGain(GetOrSetProperty getOrSet, const double value) {
	if (getOrSet == SetProperty) {
		is_SetHardwareGain(_camHandle, value, value, value, value);
	}
	int current = is_SetHardwareGain(_camHandle, IS_GET_MASTER_GAIN, 0, 0, 0);
	CameraProperty prop(PropHardwareGain, "Hardware gain");
	prop.setNumeric(current);
	return prop;
}

CameraProperty IDSCamera::_getSetPixelClock(GetOrSetProperty getOrSet, const std::string& mode) {
	if (_supportedPixelClocks.empty()) {
		int nNumberOfSupportedPixelClocks = 0;
		int nRet = is_PixelClock(_camHandle, IS_PIXELCLOCK_CMD_GET_NUMBER, &nNumberOfSupportedPixelClocks, sizeof(nNumberOfSupportedPixelClocks));
		if (nRet != IS_SUCCESS) throw std::runtime_error("unable to determine n pixel clocks");
		int clockList[150];
		nRet = is_PixelClock(_camHandle, IS_PIXELCLOCK_CMD_GET_LIST, clockList, nNumberOfSupportedPixelClocks * sizeof(int));
		if (nRet != IS_SUCCESS) throw std::runtime_error("unable to determine n pixel clocks");
		for (int i = 0; i < nNumberOfSupportedPixelClocks; i += 1) {
			char buf[128];
			sprintf(buf, "%d MHz", clockList[i]);
			_supportedPixelClocks.emplace_back(clockList[i], buf);
		}
	}

	if (getOrSet == SetProperty) {
		auto it = std::find_if(_supportedPixelClocks.cbegin(), _supportedPixelClocks.cend(), [&](const std::pair<int, std::string>& v) {
			return (v.second == mode);
		});
		if (it == _supportedPixelClocks.cend()) throw std::runtime_error("can't find required pixel clock");
		int reqClockSpeed = it->first;
		double currentExposureTime = _getExposureTime();
		is_PixelClock(_camHandle, IS_PIXELCLOCK_CMD_SET, &reqClockSpeed, sizeof(reqClockSpeed));
		_setExposureTime(currentExposureTime);	// work around issue where exposure time isn't updated correctly by the SDK
	}

	int currentClock = 0;
	is_PixelClock(_camHandle, IS_PIXELCLOCK_CMD_GET, &currentClock, sizeof(currentClock));
	auto it = std::find_if(_supportedPixelClocks.cbegin(), _supportedPixelClocks.cend(), [&](const std::pair<int, std::string>& v) {
		return (v.first == currentClock);
	});
	if (it == _supportedPixelClocks.cend()) throw std::runtime_error("can't find current pixel clock");
	const std::string& current = it->second;
	std::vector<std::string> availableOptions;
	for (const auto& i : _supportedPixelClocks) {
		availableOptions.emplace_back(i.second);
	}
	CameraProperty prop(PropPixelClock, "Pixel clock (MHz)");
	prop.setDiscrete(current, availableOptions);
	return prop;
}

CameraProperty IDSCamera::_getSetHotPixelCorrection(GetOrSetProperty getOrSet, const std::string & mode) {
	static const char* kNoCorrection = "No correction";
	static const char* kCameraCorrection = "Camera correction";
	static const char* kAdaptiveCorrection = "Adaptive correction";
	if (getOrSet == SetProperty) {
		if (mode == kNoCorrection) {
			is_HotPixel(_camHandle, IS_HOTPIXEL_DISABLE_CORRECTION, nullptr, 0);
			int enable = 0;
			is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_SET_ENABLE, &enable, sizeof(enable));
		} else if (mode == kCameraCorrection) {
			is_HotPixel(_camHandle, IS_HOTPIXEL_ENABLE_CAMERA_CORRECTION, nullptr, 0);
			int enable = 0;
			is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_SET_ENABLE, &enable, sizeof(enable));
		} else if (mode == kAdaptiveCorrection) {
			int enable = 1;
			is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_SET_ENABLE, &enable, sizeof(enable));
		} else {
			throw std::runtime_error("unknown mode setting hot pixel correction");
		}
	}
	int nMode = 0;
	is_HotPixel(_camHandle, IS_HOTPIXEL_GET_CORRECTION_MODE, &nMode, sizeof(mode));
	const char* modeStr = nullptr;
	switch (nMode) {
		case IS_HOTPIXEL_DISABLE_CORRECTION:
			modeStr = kNoCorrection;
			break;
		case IS_HOTPIXEL_ENABLE_CAMERA_CORRECTION:
			modeStr = kCameraCorrection;
			break;
	}
	is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_GET_ENABLE, &nMode, sizeof(nMode));
	if (nMode == IS_HOTPIXEL_ADAPTIVE_CORRECTION_ENABLE) {
		modeStr = kAdaptiveCorrection;
	}
	if (modeStr == nullptr) {
		throw std::runtime_error("unknown hot pixel correction mode");
	}
	CameraProperty prop(PropHotPixelCorrection, "Hot pixel correction");
	prop.setDiscrete(modeStr, { kNoCorrection, kCameraCorrection, kAdaptiveCorrection });
	return prop;
}

CameraProperty IDSCamera::_getSetAdaptiveHotPixelCorrectionMode(GetOrSetProperty getOrSet, const std::string & mode) {
	static const char* kDetectOnce = "Detect once";
	static const char* kDynamic = "Dynamic";
	static const char* kDetectOnceCluster = "Detect once (cluster)";
	static const char* kDetectDynamicCluster = "Detect dynamic (cluster)";
	if (getOrSet == SetProperty) {
		int nMode = 0;
		if (mode == kDetectOnce) {
			nMode = IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_ONCE;
		} else if (mode == kDynamic) {
			nMode = IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_DYNAMIC;
		} else if (mode == kDetectOnceCluster) {
			nMode = IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_ONCE_CLUSTER;
		} else if (mode == kDetectDynamicCluster) {
			nMode = IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_DYNAMIC_CLUSTER;
		} else {
			throw std::runtime_error("unknown mode setting adaptive hot pixel correction");
		}
		is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_SET_MODE, (void*)&nMode, sizeof(nMode));
	}
	int nMode = 0;
	is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_GET_MODE, (void*)&nMode, sizeof(nMode));
	const char* current = nullptr;
	switch (nMode) {
		case IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_ONCE:
			current = kDetectOnce;
			break;
		case IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_DYNAMIC:
			current = kDynamic;
			break;
		case IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_ONCE_CLUSTER:
			current = kDetectOnceCluster;
			break;
		case IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_DYNAMIC_CLUSTER:
			current = kDetectDynamicCluster;
			break;
		default:
			throw std::runtime_error("getting unknown adaptive correction mode");
			break;
	}
	CameraProperty prop(PropAdaptiveHotPixelCorrectionMode, "Adaptive hot pixel correction mode");
	prop.setDiscrete(current, { kDetectOnce, kDynamic, kDetectOnceCluster, kDetectDynamicCluster });
	return prop;
}

CameraProperty IDSCamera::_getSetAdaptiveHotPixelCorrectionSensitivity(GetOrSetProperty getOrSet, const double value) {
	if (getOrSet == SetProperty) {
		int nSensitivity = value;
		is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_SET_SENSITIVITY, &nSensitivity, sizeof(nSensitivity));
	}
	int sensitivity = 0;
	is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_GET_SENSITIVITY, &sensitivity, sizeof(sensitivity));
	CameraProperty prop(PropAdaptiveHotPixelSensitivity, "Adaptive hot pixel sensitivity");
	prop.setNumeric(sensitivity);
	return prop;
}

std::vector<std::shared_ptr<ImageProcessingDescriptor>> IDSCamera::_derivedGetAdditionalImageProcessingDescriptors() {
	std::vector<std::shared_ptr<ImageProcessingDescriptor>> descs;
	if (_getSensorSize() != _desiredCropSize) {
		descs.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDCrop(_desiredCropSize.first, _desiredCropSize.second)));
	}
	if (_desiredBinningFactor != 1) {
		descs.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDBin(_desiredBinningFactor)));
	}
	return descs;
}

bool IDSCamera::_haveGainBoost() const {
	int result = is_SetGainBoost(_camHandle, IS_GET_SUPPORTED_GAINBOOST);
	return (result == IS_SET_GAINBOOST_ON);
}

bool IDSCamera::_haveHotPixelCorrection() const {
	int modes;
	is_HotPixel(_camHandle, IS_HOTPIXEL_GET_SUPPORTED_CORRECTION_MODES, &modes, sizeof(modes));
	return (modes != 0);
}

void IDSCamera::_derivedStartAsyncAcquisition() {
    int err;
    std::pair<int, int> sensorSize = _getSensorSize();
    int nPixelsInImage = sensorSize.first * sensorSize.second;

    _frameBuffer.resize(nPixelsInImage * kIDSImagesInBuffer);
    _frameBufferPointers.resize(kIDSImagesInBuffer);
    _frameBufferIDs.resize(kIDSImagesInBuffer);
    for (int i = 0; i < kIDSImagesInBuffer; i += 1) {
        _frameBufferPointers.at(i) = reinterpret_cast<char*>(_frameBuffer.data() + i * nPixelsInImage);
        err = is_SetAllocatedImageMem(_camHandle, sensorSize.first, sensorSize.second, 16, _frameBufferPointers[i], &(_frameBufferIDs[i]));
        if (err != IS_SUCCESS) {
            throw std::runtime_error("unable to set allocated memory");
        }
        err = is_AddToSequence(_camHandle, _frameBufferPointers[i], _frameBufferIDs[i]);
		if (err != IS_SUCCESS) {
			throw std::runtime_error("unable to set add memory to sequence");
		}
    }
    
    err = is_InitImageQueue(_camHandle, 0);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to init image queue");
    }
    err = is_CaptureVideo(_camHandle, IS_DONT_WAIT);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to capture video");
    }
    _idOfMemoryWithOldestImage = -1;
    _ptrToMemoryWithOldestImage = nullptr;
}

void IDSCamera::_derivedAbortAsyncAcquisition() {
    int err = is_StopLiveVideo(_camHandle, IS_FORCE_VIDEO_STOP);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to stop live video");
    }

    for (int i = 0; i < kIDSImagesInBuffer; i += 1) {
        err = is_FreeImageMem(_camHandle, _frameBufferPointers.at(i), _frameBufferIDs.at(i));
        if (err != IS_SUCCESS) {
            throw std::runtime_error("unable to free image memory");
        }
    }

    _frameBuffer.clear();
}

bool IDSCamera::_waitForNewImageWithTimeout(int timeoutMillis) {
    char *imagePtr;
    int imageID;
    int err = is_WaitForNextImage(_camHandle, timeoutMillis, &imagePtr, &imageID);
    if (err == IS_TIMED_OUT) {
        return false;
    }
    if (err != IS_SUCCESS) {
        is_UnlockSeqBuf(_camHandle, imageID, imagePtr);
        throw std::runtime_error("error from is_WaitForNextImage()");
    }
    _idOfMemoryWithOldestImage = imageID;
    _ptrToMemoryWithOldestImage = imagePtr;
    return true;
}

bool IDSCamera::_derivedNewAsyncAcquisitionImageAvailable() {
    // only _waitForNewImageWithTimeout() can be implemented with the available ISD api
	return false;
}

void IDSCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
    int err = is_CopyImageMem(_camHandle, _ptrToMemoryWithOldestImage, _idOfMemoryWithOldestImage, reinterpret_cast<char*>(bufferForThisImage));
    is_UnlockSeqBuf(_camHandle, _idOfMemoryWithOldestImage, _ptrToMemoryWithOldestImage);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("error from is_WaitForNextImage()");
    }
    _idOfMemoryWithOldestImage = -1;
    _ptrToMemoryWithOldestImage = nullptr;
}

void IDSCamera::_setDefaults() {
    int err;
    err = is_ResetToDefault(_camHandle);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to reset camera to default");
    }
    err = is_SetDisplayMode(_camHandle, IS_SET_DM_DIB);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to set system memory mode");
    }
    err = is_SetColorMode(_camHandle, IS_CM_MONO16);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to set 16 bit color mode");
    }
    err = is_SetHardwareGamma(_camHandle, IS_SET_HW_GAMMA_OFF);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to disable gamma");
    }
    err = is_SetSubSampling(_camHandle, IS_SUBSAMPLING_DISABLE);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to disable subsampling");
    }

	int nEnable = 0;
	err = is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_SET_ENABLE, &nEnable, sizeof(nEnable));
	if (err != IS_SUCCESS) {
		throw std::runtime_error("unable to set hot pixel correction");
	}
	int hotPixelMode = IS_HOTPIXEL_ADAPTIVE_CORRECTION_DETECT_ONCE;
	err = is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_SET_MODE, &hotPixelMode, sizeof(hotPixelMode));
	if (err != IS_SUCCESS) {
		throw std::runtime_error("unable to set hot pixel correction");
	}
	int hotPixelSensitivity = 5;
	err = is_HotPixel(_camHandle, IS_HOTPIXEL_ADAPTIVE_CORRECTION_SET_SENSITIVITY, &hotPixelSensitivity, sizeof(hotPixelSensitivity));
	if (err != IS_SUCCESS) {
		throw std::runtime_error("unable to set hot pixel correction sensitivity");
	}
}

#endif
