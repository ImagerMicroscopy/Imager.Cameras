#include "SCConfigure.h"

#ifdef WITH_ANDORSDK3

#include <functional>

#include "AndorSDK3Camera.h"

#include "Andor/atutility.h"

#include "Utils.h"

AndorSDK3Camera::AndorSDK3Camera(const AT_H camHandle) :
    _camHandle(camHandle),
    _imageBufferSize(0)
{
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
    _getSetTriggerMode(kTriggerInternal);
    _setParameterBoolValue("MetadataEnable", true);
    _setExposureTime(0.05);
}

#endif

void AndorSDK3Camera::_derivedStartAsyncAcquisition() {
    AT_Flush(_camHandle); // remove pending buffers

    IntValue imageSize = _getParameterIntValue("ImageSizeBytes");
    size_t nBytesPerImage = imageSize.currentValue;
    size_t nUInt64PerImage = nBytesPerImage / sizeof(std::uint64_t) + 1;    // +1 in case there is truncation in the division
    _imageBufferSize = nUInt64PerImage * sizeof(std::uint64_t);

    if (_bufferMemory.size() < nUInt64PerImage * kNBufferImages) {
        _bufferMemory.resize(nUInt64PerImage * kNBufferImages);
        _imageBufferPtrs.resize(kNBufferImages);
    }
    for (size_t i = 0; i < _imageBufferPtrs.size(); ++i) {
        _imageBufferPtrs.at(i) = reinterpret_cast<std::uint16_t*>(&(_bufferMemory.at(nUInt64PerImage)));
        int err = AT_QueueBuffer(_camHandle, reinterpret_cast<std::uint8_t*>(_imageBufferPtrs.at(i)), _imageBufferSize);
        if (err) {
            throw std::runtime_error("can't queue Andor3 buffer");
        }
    }

    _sendCommand("AcquisitionStart");
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

    err = AT_ConvertBufferUsingMetadata(bufPtr, reinterpret_cast<std::uint8_t*>(bufferForThisImage), nBytes, L"Mono16");
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't convert Andor3 buffer");
    }

    IntValue imageSize = _getParameterIntValue("ImageSizeBytes");
    err = AT_QueueBuffer(_camHandle, bufPtr, _imageBufferSize);
    if (err) {
        throw std::runtime_error("can't requeue Andor3 buffer");
    }
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

    std::wstring wFeatureStr = utf8StringToWChar(featureStr);
    int selectedIdx = 0;
    err = AT_GetEnumIndex(_camHandle, wFeatureStr.c_str(), &selectedIdx);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 enum index");
    }

    wchar_t buf[256];
    err = AT_GetEnumStringByIndex(_camHandle, wFeatureStr.c_str(), selectedIdx, buf, sizeof(buf));
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor3 enum string");
    }

    return wcharStringToUtf8(buf);
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

AndorSDK3Camera::TriggerMode AndorSDK3Camera::_getSetTriggerMode(std::optional<TriggerMode> maybeMode) {
    std::string modeStr;
    int err = AT_SUCCESS;

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

std::pair<int, int> AndorSDK3Camera::_getSizeOfRawImages() const {
    IntValue width = _getParameterIntValue("SensorWidth");
    IntValue height = _getParameterIntValue("SensorHeight");

    return {width.currentValue, height.currentValue};
}

void AndorSDK3Camera::_setExposureTime(const double exposureTime) {
    _setParameterFloatValue("ExposureTime", exposureTime);
    FloatValue rateVals = _getParameterFloatValue("ExposureTime");
    _setParameterFloatValue("FrameRate", rateVals.minValue);
}

double AndorSDK3Camera::_getExposureTime() const {
    double expTime;
    FloatValue val = _getParameterFloatValue("ExposureTime");
    return val.currentValue;
}
