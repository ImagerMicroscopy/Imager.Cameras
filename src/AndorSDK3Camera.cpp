#include "SCConfigure.h"

#ifdef WITH_ANDORSDK3

#include <functional>

#include "AndorSDK3Camera.h"

#include "Andor/atutility.h"

#include "Utils.h"

AndorSDK3Camera::AndorSDK3Camera(const AT_H camHandle) :
    _camHandle(camHandle),
    _singleImageSizeInBytes(0),
    _softwareTriggeredAcquisitionRunning(false)
{
    _setDefaults();

    _cropSize = AndorSDK3Camera::_getSizeOfRawImages();
}

AndorSDK3Camera::~AndorSDK3Camera() {
    AT_Close(_camHandle);
}

std::string AndorSDK3Camera::getIdentifierStr() const {
    AT_WC szValue[64];
    int err = 0;

    err = AT_GetString(_camHandle, L"CameraModel", szValue, 64);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor 3 camera model");
    }
    std::string name = wcharStringToUtf8(szValue);

    err = AT_GetString(_camHandle, L"Serial Number", szValue, 64);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor 3 serial number");
    }
    name += "_";
    name += wcharStringToUtf8(szValue);

    return name;
}

double AndorSDK3Camera::getFrameRate() const {
    FloatValue val = _getParameterFloatValue("FrameRate");
    return val.currentValue;
}

void AndorSDK3Camera::_setDefaults() {
    _setParameterStringValue("PixelEncoding", "Mono16");
    _setParameterStringValue("CycleMode", "Continuous");
    _getSetTriggerMode_SDK(kTriggerInternal);
    _setParameterBoolValue("MetadataEnable", true);
    _setExposureTime(0.05);
}

std::vector<CameraProperty> AndorSDK3Camera::_derivedGetCameraProperties() {
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

void AndorSDK3Camera::_derivedSetCameraProperties(const std::vector<CameraProperty>& properties) {
    _stopSoftwareTriggeredAcquisitionIfRunning();

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
            default:
                throw std::runtime_error("Andor3 setting unknown property");
        }
    }
}

CameraProperty AndorSDK3Camera::_getSetPixelClock(std::optional<CameraProperty> maybeValueToSet) {
    if (maybeValueToSet.has_value()) {
        _getSetPixelClock_SDK(maybeValueToSet.value().getCurrentOption());
    }

    CameraProperty prop(PropPixelClock, "Pixel clock");
    prop.setDiscrete(_getSetPixelClock_SDK(), _getPossiblePixelClocks());
    return prop;
}

void AndorSDK3Camera::_derivedAcquireSingleImage(std::uint16_t *bufferForThisImage, int nBytes) {
    if (!_softwareTriggeredAcquisitionRunning) {
        // start a new software triggered acquisition
        _startUnboundedAsyncAcquisition(kTriggerSoftware);
        _softwareTriggeredAcquisitionRunning = true;
    }

    _sendSoftwareTrigger();

    int waitMillis = std::max(1000, static_cast<int>(_getExposureTime() * 1000.0 * 2.0));
    NewImageResult result = _waitForNewImageWithTimeout(waitMillis, bufferForThisImage, nBytes);
    if (result == NoImageBeforeTimeout) {
        throw std::runtime_error("Andor 3 timeout in single image acquisition");
    }
}

void AndorSDK3Camera::_stopSoftwareTriggeredAcquisitionIfRunning() {
    if (_softwareTriggeredAcquisitionRunning) {
        _derivedAbortAsyncAcquisition();
        _softwareTriggeredAcquisitionRunning = false;
    }
}

void AndorSDK3Camera::_derivedStartUnboundedAsyncAcquisition() {
    // if called here then we are not doing single image acquisitions,
    // so we will run in internal trigger mode
    _stopSoftwareTriggeredAcquisitionIfRunning();
    
    _startUnboundedAsyncAcquisition(kTriggerInternal);
}

void AndorSDK3Camera::_derivedAbortAsyncAcquisition() {
    _sendCommand("AcquisitionStop");
    AT_Flush(_camHandle);
}

AndorSDK3Camera::NewImageResult AndorSDK3Camera::_waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) {
    std::uint8_t* bufPtr = nullptr;
    int bufSize = 0;
    int err = AT_WaitBuffer(_camHandle, &bufPtr, &bufSize, timeoutMillis);
    if (err == AT_ERR_TIMEDOUT) {
        return NoImageBeforeTimeout;
    }
    if (err != AT_SUCCESS) {
        throw std::runtime_error("error waiting for Andor3 image");
    }

    // make sure the buffer pointer is one that we know
    const auto it = std::find(_imageBufferPtrs.cbegin(), _imageBufferPtrs.cend(), reinterpret_cast<std::uint16_t*>(bufPtr));
    if (it == _imageBufferPtrs.cend()) {
        throw std::runtime_error("unknown Andor3 buffer pointer");
    }

    auto [aoiWidth, aoiHeight] = _getSizeOfRawImages();
    if (nBytes != aoiWidth * aoiHeight * sizeof(std::uint16_t)) {
        throw std::runtime_error("buffer size doesn't match Andor3 image");
    }
    // the docs mention that the image rows may be padded so we'll use a conversion function
    AT_64 aoiStrideInBytes = 0;
    AT_GetInt(_camHandle, L"AOI Stride", &aoiStrideInBytes);
    err = AT_ConvertBuffer(bufPtr, (std::uint8_t*)bufferForThisImage, aoiWidth, aoiHeight, aoiStrideInBytes, L"Mono16", L"Mono16");
    if (err) {
        throw std::runtime_error("can't convert Andor3 buffer");
    }

    err = AT_QueueBuffer(_camHandle, bufPtr, _singleImageSizeInBytes);
    if (err) {
        throw std::runtime_error("can't requeue Andor3 buffer");
    }

    return NewImageCopied;
}

void AndorSDK3Camera::_startUnboundedAsyncAcquisition(TriggerMode triggerMode) {
    AT_Flush(_camHandle); // remove pending buffers

    _getSetTriggerMode_SDK(triggerMode);

    // we base the buffers on uint64_t to make sure that the alignment is fine, since there is some mention of this in the docs
    IntValue imageSize = _getParameterIntValue("ImageSizeBytes");
    _singleImageSizeInBytes = imageSize.currentValue;
    size_t nUInt64PerImage = _singleImageSizeInBytes / sizeof(std::uint64_t) + 1;    // +1 in case there is truncation in the division
    size_t bufferSizePerImage = nUInt64PerImage * sizeof(std::uint64_t);
    if (bufferSizePerImage < _singleImageSizeInBytes) {
        throw std::logic_error("Andor3 buffer is too small");
    }

    if (_bufferMemory.size() < nUInt64PerImage * kNBufferImages) {
        _bufferMemory.resize(nUInt64PerImage * kNBufferImages);
        _imageBufferPtrs.resize(kNBufferImages);
    }
    for (size_t i = 0; i < _imageBufferPtrs.size(); ++i) {
        _imageBufferPtrs.at(i) = reinterpret_cast<std::uint16_t*>(&(_bufferMemory.at(i * nUInt64PerImage)));
        int err = AT_QueueBuffer(_camHandle, reinterpret_cast<std::uint8_t*>(_imageBufferPtrs.at(i)), _singleImageSizeInBytes);
        if (err) {
            throw std::runtime_error("can't queue Andor3 buffer");
        }
    }

    _sendCommand("AcquisitionStart");
}

void AndorSDK3Camera::_setParameterStringValue(const std::string& featureStr, const std::string& valueStr) {
    std::wstring wFeatureStr = utf8StringToWChar(featureStr);
    std::wstring wValueStr = utf8StringToWChar(valueStr);

    int err = AT_SetEnumeratedString(_camHandle, wFeatureStr.c_str(), wValueStr.c_str());
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't set Andor3 enum string");
    }
}

std::string AndorSDK3Camera::_getSelectedParameterStringValue(const std::string& featureStr) const {
    int err = AT_SUCCESS;

    std::vector<std::string> featureOptions = _enumerateParameterStringValues(featureStr);

    std::wstring wFeatureStr = utf8StringToWChar(featureStr);
    int selectedIdx = 0;
    err = AT_GetEnumIndex(_camHandle, wFeatureStr.c_str(), &selectedIdx);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 enum index");
    }

    return featureOptions.at(selectedIdx);
}

std::vector<std::string> AndorSDK3Camera::_enumerateParameterStringValues(const std::string& featureStr) const {
    std::wstring wFeatureStr = utf8StringToWChar(featureStr);
    int count = 0;
    int err = AT_GetEnumCount(_camHandle, wFeatureStr.c_str(), &count);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 enum count");
    }

    std::vector<std::string> values;
    for (int i = 0; i < count; ++i) {
        wchar_t buf[256];
        err = AT_GetEnumStringByIndex(_camHandle, wFeatureStr.c_str(), i, buf, sizeof(buf));
        if (err != AT_SUCCESS) {
            throw std::runtime_error("can't get Andor3 enum string");
        }
        values.push_back(wcharStringToUtf8(buf));
    }
    return values;
}

void AndorSDK3Camera::_setParameterFloatValue(const std::string& featureStr, double value) {
    int err = AT_SUCCESS;
    std::wstring wFeatureStr = utf8StringToWChar(featureStr);
    err = AT_SetFloat(_camHandle, wFeatureStr.c_str(), value);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't set Andor3 float value");
    }
}

AndorSDK3Camera::FloatValue AndorSDK3Camera::_getParameterFloatValue(const std::string& featureStr) const {
    int err = AT_SUCCESS;
    std::wstring wFeatureStr = utf8StringToWChar(featureStr);

    double current, min, max;
    err = AT_GetFloat(_camHandle, wFeatureStr.c_str(), &current);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 float value");
    }
    err = AT_GetFloatMin(_camHandle, wFeatureStr.c_str(), &min);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 float min value");
    }
    err = AT_GetFloatMax(_camHandle, wFeatureStr.c_str(), &max);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 float max value");
    }

    return FloatValue(current, min, max);
}

void AndorSDK3Camera::_setParameterIntValue(const std::string& featureStr, int value) {
    int err = AT_SUCCESS;
    std::wstring wFeatureStr = utf8StringToWChar(featureStr);
    err = AT_SetInt(_camHandle, wFeatureStr.c_str(), value);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't set Andor3 int value");
    }
}

AndorSDK3Camera::IntValue AndorSDK3Camera::_getParameterIntValue(const std::string& featureStr) const {
    int err = AT_SUCCESS;
    std::wstring wFeatureStr = utf8StringToWChar(featureStr);

    std::int64_t current, min, max;
    err = AT_GetInt(_camHandle, wFeatureStr.c_str(), &current);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 int value");
    }
    err = AT_GetIntMin(_camHandle, wFeatureStr.c_str(), &min);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 int min value");
    }
    err = AT_GetIntMax(_camHandle, wFeatureStr.c_str(), &max);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 int max value");
    }

    return IntValue(current, min, max);
}

void AndorSDK3Camera::_setParameterBoolValue(const std::string& featureStr, bool value) {
    int err = AT_SUCCESS;
    std::wstring wFeatureStr = utf8StringToWChar(featureStr);
    AT_BOOL asATBool = (value) ? AT_TRUE : AT_FALSE;
    err = AT_SetBool(_camHandle, wFeatureStr.c_str(), asATBool);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't set Andor3 bool value");
    }
}

bool AndorSDK3Camera::_getParameterBoolValue(const std::string& featureStr) const {
    int err = AT_SUCCESS;
    std::wstring wFeatureStr = utf8StringToWChar(featureStr);
    AT_BOOL asATBool;
    err = AT_GetBool(_camHandle, wFeatureStr.c_str(), &asATBool);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 bool value");
    }
    return (asATBool == AT_TRUE);
}

void AndorSDK3Camera::_sendCommand(const std::string& command) {
    std::wstring wCommand = utf8StringToWChar(command);
    int err = AT_Command(_camHandle, wCommand.c_str());
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't send Andor3 software trigger");
    }
}

std::string AndorSDK3Camera::_getSetPixelClock_SDK(std::optional<std::string> maybeClock) {
    if (maybeClock.has_value()) {
        const std::string& newClock = maybeClock.value();
        _setParameterStringValue("PixelReadoutRate", newClock);
    }

    return _getSelectedParameterStringValue("PixelReadoutRate");
}

std::vector<std::string> AndorSDK3Camera::_getPossiblePixelClocks() const {
    return _enumerateParameterStringValues("PixelReadoutRate");
}

AndorSDK3Camera::TriggerMode AndorSDK3Camera::_getSetTriggerMode_SDK(std::optional<TriggerMode> maybeMode) {
    std::string modeStr;

    if (maybeMode.has_value()) {
        switch (maybeMode.value()) {
            case kTriggerInternal:
                modeStr = "Internal";
                break;
            case kTriggerSoftware:
                modeStr = "Software";
                break;
            default:
                throw std::runtime_error("unknown Andor3 trigger mode to set");
        }
        _setParameterStringValue("TriggerMode", modeStr);
    }

    std::string currValue = _getSelectedParameterStringValue("TriggerMode");
    if (currValue == "Internal") {
        return kTriggerInternal;
    } else if (currValue == "Software") {
        return kTriggerSoftware;
    } else {
        throw std::runtime_error("unknown Andor3 trigger mode received");
    }
}

void AndorSDK3Camera::_sendSoftwareTrigger() {
    _sendCommand("SoftwareTrigger");
}

std::vector<std::shared_ptr<ImageProcessingDescriptor>> AndorSDK3Camera::_derivedGetAdditionalImageProcessingDescriptors() {
    std::vector<std::shared_ptr<ImageProcessingDescriptor>> descriptors;
    if (_getSizeOfRawImages() != _cropSize) {
        descriptors.emplace_back(new IPDCrop(_cropSize.first, _cropSize.second));
    }
    return descriptors;
}

std::pair<int, int> AndorSDK3Camera::_getSizeOfRawImages() const {
    IntValue width = _getParameterIntValue("AOIWidth");
    IntValue height = _getParameterIntValue("AOIHeight");

    return {width.currentValue, height.currentValue};
}

void AndorSDK3Camera::_setExposureTime(const double exposureTime) {
    _setParameterFloatValue("ExposureTime", exposureTime);
    FloatValue rateVals = _getParameterFloatValue("FrameRate");
    _setParameterFloatValue("FrameRate", rateVals.maxValue);
}

double AndorSDK3Camera::_getExposureTime() const {
    double expTime;
    FloatValue val = _getParameterFloatValue("ExposureTime");
    return val.currentValue;
}

#endif
