#include "SCConfigure.h"

#ifdef WITH_PCO

#include "PCOCamera.h"

PCOCamera::PCOCamera(HANDLE camHandle) :
    _camHandle(camHandle),
    _pcoAPIWrapper(GetPCOAPIWrapper()),
    _camDescription({0}),
    _nextBufferToReadIndex(-1)
{
    _fetchCameraInfo();
    _initDefaults();
    _desiredCropSize = _getSensorSize();
    _desiredBinningFactor = 1;
    _numberOfImagesDelivered = 0;
}

PCOCamera::~PCOCamera() {
    for (auto waitH : _waitObjects) {
        if (waitH != nullptr) {
            CloseHandle(waitH);
        }
    }

    if (_camHandle != nullptr) {
        _pcoAPIWrapper.PCO_CloseCamera(_camHandle);
        _camHandle = nullptr;
    }
}

std::string PCOCamera::getIdentifierStr() const {
    return _camName;
}

double PCOCamera::getFrameRate() {
    DWORD time_s, time_ns;
    int pcoErr = _pcoAPIWrapper.PCO_GetCOCRuntime(_camHandle, &time_s, &time_ns);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetCOCRuntime()");
    }
    return (1.0 / (time_s + time_ns / 1.0e9));
}

std::string PCOCamera::pcoErrorAsString(const int errCode) {
    char buf[512];
    PCOAPIWrapper apiWrapper = GetPCOAPIWrapper();
    apiWrapper.PCO_GetErrorTextSDK(errCode, buf, sizeof(buf));
    return std::string(buf);
}

std::vector<CameraProperty> PCOCamera::_derivedGetCameraProperties() {
    std::vector<CameraProperty> properties;
    properties = GetStandardProperties(_getExposureTime(), _desiredCropSize,
                                       StandardCroppingOptions(_getSensorSize().first),
                                       StandardCroppingOptions(_getSensorSize().second),
                                       _desiredBinningFactor, StandardBinningOptions());

    properties.push_back(_getSetReadoutSpeed(GetProperty, std::string()));

    return properties;
}

void PCOCamera::_derivedSetCameraProperties(const std::vector<CameraProperty> &properties) {
    std::vector<CameraProperty> propsCopy(properties);

    DecodedStandardProperties decodedStandardProperties = DecodeAndRemoveStandardProperties(propsCopy);
    if (decodedStandardProperties.crop1.has_value()) {
        _desiredCropSize.first = decodedStandardProperties.crop1.value();
    }
    if (decodedStandardProperties.crop2.has_value()) {
        _desiredCropSize.second = decodedStandardProperties.crop2.value();
    }
    if (decodedStandardProperties.binningFactor.has_value()) {
        _desiredBinningFactor = decodedStandardProperties.binningFactor.value();
    }
    if (decodedStandardProperties.exposureTime.has_value()) {
        _setExposureTime(decodedStandardProperties.exposureTime.value());
    }

    for (const auto &prop : propsCopy) {
        switch (prop.getPropertyCode()) {
            case PropReadoutSpeed:
                _getSetReadoutSpeed(SetProperty, prop.getCurrentOption());
                break;
            default:
                throw std::runtime_error("setting unrecognized option");
        }
    }
}

std::pair<int, int> PCOCamera::_getSizeOfRawImages() {
    return _getSensorSize();
}

CameraProperty PCOCamera::_getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string &mode) {
    if (getOrSet == SetProperty) {
        int pixelRate = _readoutSpeeds.at(mode);
        int pcoErr = _pcoAPIWrapper.PCO_SetPixelRate(_camHandle, pixelRate);
        if (pcoErr) {
            throw std::runtime_error("Error calling PCO_SetPixelRate()");
        }
        pcoErr = _pcoAPIWrapper.PCO_ArmCamera(_camHandle);
        if (pcoErr) {
            throw std::runtime_error("Error calling PCO_ArmCamera()");
        }
    }

    std::vector<std::string> allowedSpeeds;
    for (const auto &s : _readoutSpeeds) {
        allowedSpeeds.emplace_back(s.first);
    }
    DWORD currentRate;
    int pcoErr = _pcoAPIWrapper.PCO_GetPixelRate(_camHandle, &currentRate);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetPixelRate()");
    }
    auto it = std::find_if(_readoutSpeeds.cbegin(), _readoutSpeeds.cend(), [=](const std::pair<std::string, int>& p) { return (p.second == currentRate); });
    if (it == _readoutSpeeds.cend()) {
        throw std::runtime_error("Can't find pixel rate");
    }
    std::string currentSetting = it->first;

    CameraProperty prop(PropReadoutSpeed, "Readout speed");
    prop.setDiscrete(currentSetting, allowedSpeeds);
    return prop;
}

void PCOCamera::_setExposureTime(const double exposureTime) {
    double minExposureSecs = _camDescription.dwMinExposureDESC / 1.0e9;
    double maxExposureSecs = _camDescription.dwMaxExposureDESC / 1.0e3;
    double clampedExpTime = std::min(std::max(exposureTime, minExposureSecs), maxExposureSecs);
    int pcoErr = _pcoAPIWrapper.PCO_SetDelayExposureTime(_camHandle, 0, clampedExpTime * 1.0e6, 0, 0x0001);
    if (pcoErr) {
        std::string errMsg = pcoErrorAsString(pcoErr);
        throw std::runtime_error("Error calling PCO_SetDelayExposureTime(): " + pcoErrorAsString(pcoErr));
    }
    pcoErr = _pcoAPIWrapper.PCO_ArmCamera(_camHandle);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_SetDelayExposureTime()");
    }
}

double PCOCamera::_getExposureTime() {
    DWORD dwDelay, dwExposure;
    WORD wTimeBaseDelay, wTimeBaseExposure;
    int pcoErr = _pcoAPIWrapper.PCO_GetDelayExposureTime(_camHandle, &dwDelay, &dwExposure, &wTimeBaseDelay, &wTimeBaseExposure);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetDelayExposureTime()");
    }
    return _pcoTimeToSeconds(dwExposure, wTimeBaseExposure);
}

std::vector<std::shared_ptr<ImageProcessingDescriptor>> PCOCamera::_derivedGetAdditionalImageProcessingDescriptors() {
    std::vector<std::shared_ptr<ImageProcessingDescriptor>> descs;
    if (_getSensorSize() != _desiredCropSize) {
        descs.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDCrop(_desiredCropSize.first, _desiredCropSize.second)));
    }
    if (_desiredBinningFactor != 1) {
        descs.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDBin(_desiredBinningFactor)));
    }
    return descs;
}

void PCOCamera::_derivedStartBoundedAsyncAcquisition(std::uint64_t nImagesToAcquire) {
    std::pair<int, int> imageSize = _getSensorSize();
    int nPixelsInImage = imageSize.first * imageSize.second;

    if (nImagesToAcquire > (uint64_t)std::numeric_limits<std::uint32_t>::max()) {
        nImagesToAcquire = std::numeric_limits<std::uint32_t>::max();
    }

    if (_frameBuffer.empty()) {
        auto sensorSize = _getSensorSize();
        _frameBuffer.resize(sensorSize.first * sensorSize.second * kPCOImagesInBuffer);
    }
    if (_waitObjects.empty()) {
        _waitObjects.resize(kPCOImagesInBuffer);
        for (auto &waitH : _waitObjects) {
            waitH = CreateEventA(nullptr, FALSE, FALSE, nullptr);
            if (waitH == nullptr) {
                throw std::runtime_error("Can't create wait object");
            }
        }
        _bufferStatuses.resize(kPCOImagesInBuffer);
    }
    for (auto &waitH : _waitObjects) {
        ResetEvent(waitH);
    }
    _throwIfPCOError(_pcoAPIWrapper.PCO_SetImageParameters(_camHandle, imageSize.first, imageSize.second, IMAGEPARAMETERS_READ_WHILE_RECORDING, nullptr, 0));
    _throwIfPCOError(_pcoAPIWrapper.PCO_SetAcquireModeEx(_camHandle, ACQUIRE_MODE_IMAGE_SEQUENCE, nImagesToAcquire, nullptr));
    _pcoAPIWrapper.PCO_SetStorageMode(_camHandle, STORAGE_MODE_FIFO_BUFFER);
    _throwIfPCOError(_pcoAPIWrapper.PCO_SetTimestampMode(_camHandle, 0));
    _throwIfPCOError(_pcoAPIWrapper.PCO_ArmCamera(_camHandle));

    _throwIfPCOError(_pcoAPIWrapper.PCO_SetRecordingState(_camHandle, 1));

    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    _throwIfPCOError(_pcoAPIWrapper.PCO_SetAcquireControl(_camHandle, ACQUIRE_CONTROL_FORCE_LOW, nullptr, 0));
    _throwIfPCOError(_pcoAPIWrapper.PCO_SetAcquireControl(_camHandle, ACQUIRE_CONTROL_FORCE_HIGH, nullptr, 0));
    _throwIfPCOError(_pcoAPIWrapper.PCO_SetAcquireControl(_camHandle, ACQUIRE_CONTROL_OFF, NULL, 0));

    for (int i = 0; i < kPCOImagesInBuffer; i += 1) {
        std::uint16_t *thisImagePtr = _frameBuffer.data() + i * nPixelsInImage;
        _throwIfPCOError(_pcoAPIWrapper.PCO_AddBufferExtern(_camHandle, _waitObjects.at(i), 1, 0, 0, 0, thisImagePtr, nPixelsInImage * sizeof(std::uint16_t), &(_bufferStatuses.at(i))));
    }
    _nextBufferToReadIndex = 0;

    _numberOfImagesDelivered = 0;
}

void PCOCamera::_derivedAbortAsyncAcquisition() {
    _throwIfPCOError(_pcoAPIWrapper.PCO_SetRecordingState(_camHandle, 0));
    _throwIfPCOError(_pcoAPIWrapper.PCO_CancelImages(_camHandle));
}

BaseCameraClass::NewImageResult PCOCamera::_waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t *bufferForThisImage, int nBytes) {
    int result = WaitForSingleObject(_waitObjects.at(_nextBufferToReadIndex), timeoutMillis);
    if ((result != WAIT_OBJECT_0) && (result != WAIT_TIMEOUT)) {
        throw std::runtime_error("Unexpected return in WaitForSingleObject()");
    }

    if (result == WAIT_TIMEOUT) {
        return NoImageBeforeTimeout;
    }

    auto imageSize = _getSensorSize();
    int nPixelsInImage = imageSize.first * imageSize.second;
    int nBytesPerImage = nPixelsInImage * sizeof(std::uint16_t);
    std::uint16_t *thisImagePtr = _frameBuffer.data() + _nextBufferToReadIndex * nPixelsInImage;
    if (nBytes != nBytesPerImage) {
        throw std::runtime_error("buffer of invalid size for PCO");
    }

    memcpy(bufferForThisImage, thisImagePtr, nBytesPerImage);

    ResetEvent(_waitObjects.at(_nextBufferToReadIndex));
    int pcoErr = _pcoAPIWrapper.PCO_AddBufferExtern(_camHandle, _waitObjects.at(_nextBufferToReadIndex), 1, 0, 0, 0, thisImagePtr, nPixelsInImage * sizeof(std::uint16_t), &(_bufferStatuses.at(_nextBufferToReadIndex)));
    if (pcoErr) {
        std::string errMsg = pcoErrorAsString(pcoErr);
        throw std::runtime_error("Error calling PCO_AddBufferExtern(): " + errMsg);
    }

    _nextBufferToReadIndex = (_nextBufferToReadIndex + 1) % kPCOImagesInBuffer;
    _numberOfImagesDelivered += 1;

    return NewImageCopied;
}

void PCOCamera::_fetchCameraInfo() {
    _camDescription.wSize = sizeof(_camDescription);
    int pcoErr = _pcoAPIWrapper.PCO_GetCameraDescription(_camHandle, &_camDescription);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetCameraDescription()");
    }

    PCO_General generalDescription = {0};
    generalDescription.wSize = sizeof(generalDescription);
    generalDescription.strCamType.wSize = sizeof(generalDescription.strCamType);
    pcoErr = _pcoAPIWrapper.PCO_GetGeneral(_camHandle, &generalDescription);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetGeneral()");
    }
    PCO_CameraType camType = {0};
    camType.wSize = sizeof(camType);
    pcoErr = _pcoAPIWrapper.PCO_GetCameraType(_camHandle, &camType);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetCameraType()");
    }

    char camName[64];
    pcoErr = _pcoAPIWrapper.PCO_GetCameraName(_camHandle, camName, sizeof(camName));
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetCameraName()");
    }
    char fullCamName[128];
    sprintf(fullCamName, "PCO_%d", (int)(camType.dwSerialNumber));
    _camName = fullCamName;

    WORD xResAct, yResAct, xResMax, yResMax;
    pcoErr = _pcoAPIWrapper.PCO_GetSizes(_camHandle, &xResAct, &yResAct, &xResMax, &yResMax);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetSizes()");
    }
    _sensorSize.first = xResMax;
    _sensorSize.second = yResMax;

    for (int i = 0; i < 4; i += 1) {
        int readoutSpeed = _camDescription.dwPixelRateDESC[i];
        if (readoutSpeed == 0) {
            continue;
        }
        char buf[64];
        sprintf(buf, "%g MHz", (double)readoutSpeed / 1.0e6);
        _readoutSpeeds.emplace(std::string(buf), readoutSpeed);
    }
}

void PCOCamera::_initDefaults() {
    _pcoAPIWrapper.PCO_SetRecordingState(_camHandle, 0);

    int pcoErr = _pcoAPIWrapper.PCO_ResetSettingsToDefault(_camHandle);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_ResetSettingsToDefault()");
    }

    WORD metaDataSize, metaDataVersion;
    pcoErr = _pcoAPIWrapper.PCO_SetMetaDataMode(_camHandle, 0, &metaDataSize, &metaDataVersion);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_SetMetaDataMode()");
    }

    pcoErr = _pcoAPIWrapper.PCO_ArmCamera(_camHandle);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_ArmCamera()");
    }

    DWORD cameraWarning, cameraError, cameraStatus;
    pcoErr = _pcoAPIWrapper.PCO_GetCameraHealthStatus(_camHandle, &cameraWarning, &cameraError, &cameraStatus);
    if (pcoErr) {
        throw std::runtime_error("Error calling PCO_GetCameraHealthStatus()");
    }

    _setExposureTime(0.1);
}

double PCOCamera::_pcoTimeToSeconds(DWORD time, DWORD timeBase) const {
    double timeSeconds = time;
    switch (timeBase) {
        case 0x0000:
            timeSeconds /= 1.0e9;
            break;
        case 0x0001:
            timeSeconds /= 1.0e6;
            break;
        case 0x0002:
            timeSeconds /= 1.0e3;
            break;
        default:
            throw std::runtime_error("Unknown time base in _pcoTimeToSeconds()");
    }
    return timeSeconds;
}

#endif
