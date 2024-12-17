#include "SCConfigure.h"

#ifdef WITH_IDS_PEAK

#include "IDSPeakCamera.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "Utils.h"

IDSPeakCamera::IDSPeakCamera(const peak_camera_descriptor& camDescriptor) :
    _camDescriptor(camDescriptor),
    _camHandle(nullptr),
    _nextExpectedImageInSequenceIdx(0)
{
    peak_status status = PEAK_STATUS_SUCCESS;

    if(peak_Camera_GetAccessStatus(camDescriptor.cameraID) != PEAK_ACCESS_READWRITE) {
        throw std::runtime_error("camera does not support read/write access");
    }

    peak_camera_handle camHandle = nullptr;
    status = peak_Camera_Open(camDescriptor.cameraID, &camHandle);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't open IDS Peak camera");
    }
    _camHandle = camHandle;

    status = peak_Camera_ResetToDefaultSettings(_camHandle);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("couldn't reset IDS Peak camera to default settings");
    }

    _cropSize = _getSizeOfRawImages();
}

IDSPeakCamera::~IDSPeakCamera() {
    if (_camHandle != nullptr) {
        peak_Camera_Close(_camHandle);
    }
}

std::string IDSPeakCamera::getIdentifierStr() const {
    std::string id("IDS_");
    id += _camDescriptor.modelName;
    id += "_";
    id += _camDescriptor.serialNumber;
    return id;
}

double IDSPeakCamera::getFrameRate() const {
    double rate = 0.0;
    peak_status status = peak_FrameRate_Get(_camHandle, &rate);
    if(PEAK_ERROR(status)) {
        throw std::runtime_error("can't get IDS Peak frame rate");
    }
    return rate;
}

std::vector<CameraProperty> IDSPeakCamera::_derivedGetCameraProperties() {
    double exposureTime = _getExposureTime();
    std::vector<std::pair<int, int>> allowableCropping = StandardCroppingOptions(_getSizeOfRawImages());
    int currentBinning = 1;
    std::vector<int> allowableBinning({1});

    std::vector<CameraProperty> properties = GetStandardProperties(exposureTime, _cropSize, 
                                                                   allowableCropping, currentBinning, allowableBinning);
    properties.push_back(_getSetPixelClock());
    //properties.push_back(_getSetGain());

    return properties;
}

void IDSPeakCamera::_derivedSetCameraProperties(const std::vector<CameraProperty> &properties) {
    auto propsCopy = properties;

    auto [maybeExposureTime, maybeCropping, maybeBinning] = DecodeAndRemoveStandardProperties(propsCopy);
    if (maybeExposureTime.has_value()) {
        _setExposureTime(maybeExposureTime.value());
    }
    if (maybeCropping.has_value()) {
        _cropSize = maybeCropping.value();
    }

    for (const CameraProperty& prop : propsCopy) {
        switch (prop.getPropertyCode()) {
            case PropPixelClock:
                _getSetPixelClock(prop);
                break;
            case PropGain:
                _getSetGain(prop);
                break;
            default:
                throw std::runtime_error("IDS Peak setting unknown property");
        }
    }
}

CameraProperty IDSPeakCamera::_getSetPixelClock(std::optional<CameraProperty> maybeValueToSet) {
    peak_status status = PEAK_STATUS_SUCCESS;

    bool canAccess = false;
    peak_access_status accessStatus = peak_PixelClock_GetAccessStatus(_camHandle);
    canAccess = (accessStatus == PEAK_ACCESS_READWRITE);

    bool hasRange = false;
    double minPixelClock_MHz, maxPixelClock_MHz, incPixelClock_MHz;
    if (peak_PixelClock_HasRange(_camHandle) && canAccess) {
        hasRange = true;
        status = peak_PixelClock_GetRange(_camHandle, &minPixelClock_MHz, &maxPixelClock_MHz, &incPixelClock_MHz);
        if (PEAK_ERROR(status)) {
            throw std::runtime_error("can't get IDS Peak pixel clock range");
        }
    }

    if (maybeValueToSet.has_value() && canAccess) {
        const CameraProperty& propToSet = maybeValueToSet.value();
        double clockToSet = 0.0;
        if (hasRange) {
            clockToSet = clamp(propToSet.getValue(), minPixelClock_MHz, maxPixelClock_MHz);
        } else {
            const std::string& setting = propToSet.getCurrentOption();
            clockToSet = atof(setting.c_str());
        }
        status = peak_PixelClock_Set(_camHandle, clockToSet);
        if (PEAK_ERROR(status)) {
            throw std::runtime_error("can't set IDS Peak pixel clock");
        }
    }

    CameraProperty currentSetting(PropPixelClock, "Pixel clock (MHz)");
    double currentClock = 0.0;
    status = peak_PixelClock_Get(_camHandle, &currentClock);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't get IDS Peak pixel clock");
    }

    if (hasRange || !canAccess) {
        currentSetting.setNumeric(currentClock);
    } else{
        std::stringstream ss;
        ss << currentClock;
        std::string currentClockStr = ss.str();

        size_t nVals = 100;
        std::vector<double> availableClocks(nVals);
        status = peak_PixelClock_GetList(_camHandle, availableClocks.data(), &nVals);
        if (PEAK_ERROR(status)) {
            throw std::runtime_error("can't get IDS Peak pixel clock list");
        }
        availableClocks.resize(nVals);

        std::vector<std::string> optionsList(nVals);
        std::transform(availableClocks.cbegin(), availableClocks.cend(), optionsList.begin(), [](double c) -> std::string {
            std::stringstream ss;
            ss << c;
            return ss.str();
        });

        if (std::find(optionsList.cbegin(), optionsList.cend(), currentClockStr) == optionsList.cend()) {
            throw std::runtime_error("cannot find IDS Peak discrete pixel clock string");
        }

        currentSetting.setDiscrete(currentClockStr, optionsList);
    }

    return currentSetting;
}

CameraProperty IDSPeakCamera::_getSetGain(std::optional<CameraProperty> maybeValueToSet) {
    peak_status status = PEAK_STATUS_SUCCESS;

    double minGain, maxGain, incGain;
    status = peak_Gain_GetRange(_camHandle, PEAK_GAIN_TYPE_ANALOG, PEAK_GAIN_CHANNEL_RED, 
                                &minGain, &maxGain, &incGain);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't get IDS Peak gain range");
    }

    if (maybeValueToSet.has_value()) {
        const CameraProperty& propToSet = maybeValueToSet.value();
        double gainToSet = clamp(propToSet.getValue(), minGain, maxGain);
        status = peak_Gain_Set(_camHandle, PEAK_GAIN_TYPE_ANALOG, PEAK_GAIN_CHANNEL_RED, gainToSet);
        if (PEAK_ERROR(status)) {
            throw std::runtime_error("can't set IDS Peak gain");
        }
    }

    double currentGain = 0.0;
    status = peak_Gain_Get(_camHandle, PEAK_GAIN_TYPE_ANALOG, PEAK_GAIN_CHANNEL_RED, &currentGain);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't get IDS Peak gain");
    }

    CameraProperty prop(PropGain, "Analog gain");
    prop.setNumeric(currentGain);
    return prop;
}

std::pair<int, int> IDSPeakCamera::_getSizeOfRawImages() const {
    peak_size pSize;
    peak_status status = peak_ROI_Size_Get(_camHandle, &pSize);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't get IDS Peak ROI size");
    }

    return std::make_pair(pSize.width, pSize.height);
}

void IDSPeakCamera::_setExposureTime(const double exposureTime) {
    peak_status status = PEAK_STATUS_SUCCESS;

    double minFrameRate, maxFrameRate, frameRateStep;
    status = peak_FrameRate_GetRange(_camHandle, &minFrameRate, &maxFrameRate, &frameRateStep);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't get IDS Peak rate");
    }
    double frameRate = clamp(1.0 / exposureTime, minFrameRate, maxFrameRate);
    status = peak_FrameRate_Set(_camHandle, frameRate);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't set IDS Peak frame rate");
    }
    
    double minExp, maxExp, expStep;
    status = peak_ExposureTime_GetRange(_camHandle, &minExp, &maxExp, &expStep);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't set query IDS Peak exposure range");
    }

    double exposureTime_us = clamp(exposureTime * 1.0e6, minExp, maxExp);
    status = peak_ExposureTime_Set(_camHandle, exposureTime_us);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't set IDS Peak exposure time");
    }
}

double IDSPeakCamera::_getExposureTime() const {
    double exposureTime = 0.0;
    peak_status status = peak_ExposureTime_Get(_camHandle, &exposureTime);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't get IDS Peak exposure time");
    }
    exposureTime /= 1.0e6;
    return exposureTime;
}

std::vector<std::shared_ptr<ImageProcessingDescriptor>> IDSPeakCamera::_derivedGetAdditionalImageProcessingDescriptors() {

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> descriptors;

    if (_getSizeOfRawImages() != _cropSize) {
        descriptors.emplace_back(new IPDCrop(_cropSize.first, _cropSize.second));
    }
    
    return descriptors;
}

void IDSPeakCamera::_derivedStartAsyncAcquisition() {
    peak_status status = PEAK_STATUS_SUCCESS;
    status = peak_Acquisition_Start(_camHandle, PEAK_INFINITE);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't get start IDS Peak async acquisition");
    }

    _nextExpectedImageInSequenceIdx = 0;
}

void IDSPeakCamera::_derivedAbortAsyncAcquisition() {
    peak_status status = peak_Acquisition_Stop(_camHandle);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't get stop IDS Peak async acquisition");
    }
}

BaseCameraClass::NewImageResult IDSPeakCamera::_waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) {
    peak_frame_handle peakFrameH = nullptr;
    peak_status status = peak_Acquisition_WaitForFrame(_camHandle, timeoutMillis, &peakFrameH);
    if (status == PEAK_STATUS_TIMEOUT) {
        return NoImageBeforeTimeout;
    }
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't wait for IDS Peak frame");
    }

    if (!peak_Frame_IsComplete(peakFrameH)) {
        peak_Frame_Release(_camHandle, peakFrameH);
        throw std::runtime_error("IDS Peak frame incomplete");
    }

    peak_frame_info frameInfo;
    status = peak_Frame_GetInfo(peakFrameH, &frameInfo);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't read IDS Peak frame info");
    }

    if (frameInfo.frameID != _nextExpectedImageInSequenceIdx) {
        throw std::runtime_error("IDS Peak appears to have dropped an image");
    }
    _nextExpectedImageInSequenceIdx += 1;

    int nBytesPerPixel;
    switch (frameInfo.pixelFormat) {
        case PEAK_PIXEL_FORMAT_MONO8:
            nBytesPerPixel = 1;
            break;
        case PEAK_PIXEL_FORMAT_MONO10:
        case PEAK_PIXEL_FORMAT_MONO12:
            nBytesPerPixel = 2;
            break;
        default:
            throw std::runtime_error("unsupported pixel format for IDS Peak");
            break;
    }

    int nPixels = (int)frameInfo.bytesWritten / nBytesPerPixel;
    if (nBytes / 2 != nPixels) {    // assume UINT16 pixels in output
        throw std::runtime_error("IDS peak format not UINT16");
    }

    uint8_t* bufAddress = frameInfo.buffer.memoryAddress;
    switch (nBytesPerPixel) {
        case 1:
            for (int i = 0; i < nPixels; ++i) {
                bufferForThisImage[i] = bufAddress[i];
            }
            break;
        case 2:
            memcpy(bufferForThisImage, bufAddress, nBytes);
            break;
        default:
            throw std::runtime_error("invalid bytes per pixel for IDS Peak");
    }

    peak_Frame_Release(_camHandle, peakFrameH);
    
    return NewImageCopied;
}

#endif
