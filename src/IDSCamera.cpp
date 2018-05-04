#include "SCConfigure.h"

#ifdef WITH_IDS

#include "IDSCamera.h"

IDSCamera::IDSCamera(HIDS camHandle) :
    _camHandle(camHandle)
  , _idOfMemoryWithOldestImage(-1)
  , _ptrToMemoryWithOldestImage(nullptr)
{
	_setDefaults();
}

IDSCamera::~IDSCamera() {
    is_ExitCamera(_camHandle);
}

void IDSCamera::setExposureTime(const double exposureTime) {
    double desiredFrameRate = 1.0 / exposureTime;
    double actualFrameRate = 0.0;
    is_SetFrameRate(_camHandle, desiredFrameRate, &actualFrameRate);
    double reqExposureTime = 0.0; // If 0 is passed, the exposure time is set to the maximum value of 1/frame rate.
    is_Exposure(_camHandle, IS_EXPOSURE_CMD_SET_EXPOSURE, &reqExposureTime, sizeof(reqExposureTime));
}

void IDSCamera::setEMGain(const double emGain) {
    int gain = emGain;
    is_SetHardwareGain(_camHandle, gain, 0, 0, 0);
}

double IDSCamera::getExposureTime() const {
    double exposureTimeMillis;
    int err = is_Exposure(_camHandle, IS_EXPOSURE_CMD_GET_EXPOSURE, &exposureTimeMillis, sizeof(exposureTimeMillis));
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to get IDS exposure time");
    }
    return (exposureTimeMillis / 1000.0);
}

double IDSCamera::getEMGain() const {
    double emGain;
    emGain = is_SetHardwareGain(_camHandle, IS_GET_MASTER_GAIN, 0, 0, 0);
    return emGain;
}

std::string IDSCamera::getIdentifierStr() const {
    CAMINFO camInfo;
    int err = is_GetCameraInfo(_camHandle, &camInfo);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to get IDS camera info");
    }
    std::string idStr("IDS with serial number ");
    idStr += std::string(camInfo.SerNo);
    return idStr;
}

double IDSCamera::getTemperature() const {
    return 0.0;
}

double IDSCamera::getTemperatureSetpoint() const {
    return 0.0;
}

std::pair<int, int> IDSCamera::getSensorSize() const {
    IS_RECT rect;
    int err = is_AOI(_camHandle, IS_AOI_IMAGE_GET_AOI, &rect, sizeof(rect));
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to get AOI");
    }
    return std::pair<int, int>(rect.s32Width, rect.s32Height);
}

void IDSCamera::_derivedStartAsyncAcquisition() {
    int err;
    std::pair<int, int> sensorSize = getSensorSize();
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
    err = is_SetGainBoost(_camHandle, IS_SET_GAINBOOST_ON);
    if (err != IS_SUCCESS) {
        throw std::runtime_error("unable to set gain boost mode");
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
