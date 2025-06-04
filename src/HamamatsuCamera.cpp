#include "SCConfigure.h"

#ifdef WITH_HAMAMATSU

#include "HamamatsuCamera.h"

#include "Hamamatsu/dcamprop.h"

HamamatsuCamera::HamamatsuCamera(HDCAM camHandle) :
    _camHandle(camHandle),
    _apiWrapper(GetHamamatsuAPIWrapper()),
    _softwareTriggeredAcquisitionRunning(false),
    _camWaitHandle(nullptr),
    _numberOfImagesDelivered(0)
{
    std::string fullName = _getDCAMString(_camHandle, DCAM_IDSTR_MODEL);
    auto pos = fullName.find(' ');
    if (pos != std::string::npos) {
        _camName = fullName.substr(0, pos);
    }
    else {
        _camName = fullName;
    }
    std::string serialNumber = _getDCAMString(_camHandle, DCAM_IDSTR_CAMERAID);
    _camName += "_" + serialNumber.substr(5);
    
    _setDefaults();
}

HamamatsuCamera::~HamamatsuCamera() {
    if (_camHandle != nullptr) {
        _apiWrapper.dcamdev_close(_camHandle);
        _camHandle = nullptr;
    }
}

std::string HamamatsuCamera::getIdentifierStr() const {
    return _camName;
}

double HamamatsuCamera::getFrameRate() {
    return _getPropertyValue(_camHandle, DCAM_IDPROP_INTERNALFRAMERATE);
}

std::vector<CameraProperty> HamamatsuCamera::_derivedGetCameraProperties() {
    std::vector<CameraProperty> properties;
    properties = GetStandardProperties(_getExposureTime(), _getImageCrop(), StandardCroppingOptions(_getSensorSize().first),
                                       StandardCroppingOptions(_getSensorSize().second), _getBinningFactor(), { 1, 2, 4 });

    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_CCDMODE)) {
        properties.push_back(_getSetEMMode(GetProperty, std::string()));
    }
    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_SENSITIVITY)) {
        properties.push_back(_getSetEMGain(GetProperty, 0.0));
    }
    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_READOUTSPEED)) {
        properties.push_back(_getSetReadoutSpeed(GetProperty, std::string()));
    }
    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_SENSORCOOLER)) {
        properties.push_back(_getSetCoolerOn(GetProperty, std::string()));
    }
    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_SENSORTEMPERATURETARGET)) {
        properties.push_back(_getSetTemperatureSetPoint(GetProperty, 0.0));
    }
    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_TRIGGERSOURCE)) {
        properties.push_back(_getSetTriggerSource(GetProperty, std::string()));
    }
    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_TRIGGER_MODE)) {
        properties.push_back(_getSetTriggerMode(GetProperty, std::string()));
    }
    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_TRIGGERACTIVE)) {
        properties.push_back(_getSetTriggerActive(GetProperty, std::string()));
    }
    if (_propertyIsSupported(_camHandle, DCAM_IDPROP_TRIGGERPOLARITY)) {
        properties.push_back(_getSetTriggerPolarity(GetProperty, std::string()));
    }

    return properties;
}

void HamamatsuCamera::_derivedSetCameraProperties(const std::vector<CameraProperty>& properties) {
    std::vector<CameraProperty> propsCopy(properties);

    _stopSoftwareTriggeredAcquisitionIfRunning();

    DecodedStandardProperties decodedStandardProperties = DecodeAndRemoveStandardProperties(propsCopy);

    std::pair<int, int> currentCrop = _getImageCrop();
    bool haveCrop = false;
    if (decodedStandardProperties.crop1.has_value()) {
        currentCrop.first = decodedStandardProperties.crop1.value();
        haveCrop = true;
    }
    if (decodedStandardProperties.crop2.has_value()) {
        currentCrop.second = decodedStandardProperties.crop2.value();
        haveCrop = true;
    }
    if (haveCrop) {
        _setImageCrop(currentCrop);
    }
    if (decodedStandardProperties.binningFactor.has_value()) {
        _setBinningFactor(decodedStandardProperties.binningFactor.value());
    }
    if (decodedStandardProperties.exposureTime.has_value()) {
        _setExposureTime(decodedStandardProperties.exposureTime.value());
    }

    for (const auto& prop : propsCopy) {
        switch (prop.getPropertyCode()) {
            case PropEMMode:
                _getSetEMMode(SetProperty, prop.getCurrentOption());
                break;
            case PropEMGain:
                _getSetEMGain(SetProperty, prop.getValue());
                break;
            case PropReadoutSpeed:
                _getSetReadoutSpeed(SetProperty, prop.getCurrentOption());
                break;
            case PropTemperatureSetPoint:
                _getSetTemperatureSetPoint(SetProperty, prop.getValue());
                break;
            case PropCoolerOn:
                _getSetCoolerOn(SetProperty, prop.getCurrentOption());
                break;
            case PropTriggerSource:
                _getSetTriggerSource(SetProperty, prop.getCurrentOption());
                break;
            case PropTriggerMode:
                _getSetTriggerMode(SetProperty, prop.getCurrentOption());
                break;
            case PropTriggerActive:
                _getSetTriggerActive(SetProperty, prop.getCurrentOption());
                break;
            case PropTriggerPolarity:
                _getSetTriggerPolarity(SetProperty, prop.getCurrentOption());
                break;
            default:
                throw std::runtime_error("setting unrecognized option");
        }
    }
}

bool HamamatsuCamera::_derivedIsConfiguredForHardwareTriggering() {
    int currentSource = _getPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE);
    return ((currentSource != DCAMPROP_TRIGGERSOURCE__INTERNAL) && (currentSource != DCAMPROP_TRIGGERSOURCE__SOFTWARE));
}

std::pair<int, int> HamamatsuCamera::_getSizeOfRawImages() {
    std::pair<int, int> size;
    size.first = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_WIDTH);
    size.second = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_HEIGHT);
    return size;
}

CameraProperty HamamatsuCamera::_getSetEMMode(GetOrSetProperty getOrSet, const std::string & mode) {
    static const char* kEMMode = "EM-CCD";
    static const char* kCCDMode = "CCD";
    if (getOrSet == SetProperty) {
        if (mode == kEMMode) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_CCDMODE, DCAMPROP_CCDMODE__EMCCD);
        } else if (mode == kCCDMode) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_CCDMODE, DCAMPROP_CCDMODE__NORMALCCD);
        }
    }
    CameraProperty prop(PropEMMode, kEMMode);
    double currentSetting = _getPropertyValue(_camHandle, DCAM_IDPROP_CCDMODE);
    std::string currentSettingStr = (currentSetting == DCAMPROP_CCDMODE__EMCCD) ? kEMMode : kCCDMode;
    prop.setDiscrete(currentSettingStr, { kEMMode, kCCDMode });
    return prop;
}

CameraProperty HamamatsuCamera::_getSetEMGain(GetOrSetProperty getOrSet, const double value) {
    if (getOrSet == SetProperty) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_SENSITIVITY, value, true);
    }
    CameraProperty prop(PropEMGain, "EM Gain");
    prop.setNumeric(_getPropertyValue(_camHandle, DCAM_IDPROP_SENSITIVITY));
    return prop;
}

CameraProperty HamamatsuCamera::_getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string & mode) {
    if (getOrSet == SetProperty) {
        double value = mode.at(0) - 48;
        _setPropertyValue(_camHandle, DCAM_IDPROP_READOUTSPEED, value);
    }

    int minVal, maxVal;
    std::tie(minVal, maxVal) = _getPropertyLimits(_camHandle, DCAM_IDPROP_READOUTSPEED);
    std::vector<std::string> allowedSpeeds;
    for (int i = minVal; i <= maxVal; i += 1) {
        allowedSpeeds.emplace_back(1, (char)i + 48);
    }
    CameraProperty prop(PropReadoutSpeed, "Readout speed");
    prop.setDiscrete(std::string(1, (char)_getPropertyValue(_camHandle, DCAM_IDPROP_READOUTSPEED) + 48), allowedSpeeds);
    return prop;
}

CameraProperty HamamatsuCamera::_getSetTemperatureSetPoint(GetOrSetProperty getOrSet, const double setPoint) {
    if (getOrSet == SetProperty) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_SENSORTEMPERATURETARGET, setPoint, true);
    }
    CameraProperty prop(PropTemperatureSetPoint, "Temperature setpoint");
    prop.setNumeric(_getPropertyValue(_camHandle, DCAM_IDPROP_SENSORTEMPERATURETARGET));
    return prop;
}

CameraProperty HamamatsuCamera::_getSetCoolerOn(GetOrSetProperty getOrSet, const std::string & mode) {
    static const char* kCoolerOn = "ON";
    static const char* kCoolerOff = "OFF";
    if (getOrSet == SetProperty) {
        if (mode == kCoolerOn) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_SENSORCOOLER, DCAMPROP_SENSORCOOLER__ON);
        } else if (mode == kCoolerOff) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_SENSORCOOLER, DCAMPROP_SENSORCOOLER__OFF);
        } else {
            throw std::runtime_error("unknown argument to getSetCoolerOn()");
        }
    }
    const char* actual = (_getPropertyValue(_camHandle, DCAM_IDPROP_SENSORCOOLER) == DCAMPROP_SENSORCOOLER__ON) ? kCoolerOn : kCoolerOff;
    CameraProperty prop(PropCoolerOn, "Cooler");
    prop.setDiscrete(actual, { kCoolerOn, kCoolerOff });
    return prop;
}

CameraProperty HamamatsuCamera::_getSetTriggerSource(GetOrSetProperty getOrSet, const std::string& mode) {
    static const char* kInternal = "Internal";
    static const char* kExternal = "External";

    if (getOrSet == SetProperty) {
        if (mode == kInternal) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__INTERNAL);
        } else if (mode == kExternal) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__EXTERNAL);
        } else {
            throw std::runtime_error("unknown argument to _getSetTriggerSource()");
        }
    }
    int currentSource = _getPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE);
    const char* actual = nullptr;
    switch (currentSource) {
        case DCAMPROP_TRIGGERSOURCE__INTERNAL:
        case DCAMPROP_TRIGGERSOURCE__SOFTWARE:
            actual = kInternal;
            break;
        case DCAMPROP_TRIGGERSOURCE__EXTERNAL:
            actual = kExternal;
            break;
        default:
            throw std::runtime_error("_getSetTriggerSource() but unknown source");
            break;
    }
    CameraProperty prop(PropTriggerSource, "Trigger source");
    prop.setDiscrete(actual, { kInternal, kExternal });
    return prop;
}

CameraProperty HamamatsuCamera::_getSetTriggerMode(GetOrSetProperty getOrSet, const std::string& mode) {
    static const char* kNormal = "Normal";
    static const char* kStart = "Start";

    if (getOrSet == SetProperty) {
        if (mode == kNormal) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGER_MODE, DCAMPROP_TRIGGER_MODE__NORMAL);
        } else if (mode == kStart) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGER_MODE, DCAMPROP_TRIGGER_MODE__START);
        } else {
            throw std::runtime_error("unknown argument to _getSetTriggerMode()");
        }
    }
    int currentSource = _getPropertyValue(_camHandle, DCAM_IDPROP_TRIGGER_MODE);
    const char* actual = nullptr;
    switch (currentSource) {
        case DCAMPROP_TRIGGER_MODE__NORMAL:
            actual = kNormal;
            break;
        case DCAMPROP_TRIGGER_MODE__START:
            actual = kStart;
            break;
        default:
            throw std::runtime_error("_getSetTriggerMode() but unknown mode");
            break;
    }
    CameraProperty prop(PropTriggerMode, "Trigger mode");
    prop.setDiscrete(actual, { kNormal, kStart });
    return prop;
}

CameraProperty HamamatsuCamera::_getSetTriggerActive(GetOrSetProperty getOrSet, const std::string& mode) {
    static const char* kEdge = "Edge";
    static const char* kLevel = "Level";

    if (getOrSet == SetProperty) {
        if (mode == kEdge) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERACTIVE, DCAMPROP_TRIGGERACTIVE__EDGE);
        } else if (mode == kLevel) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERACTIVE, DCAMPROP_TRIGGERACTIVE__LEVEL);
        } else {
            throw std::runtime_error("unknown argument to _getSetTriggerActive()");
        }
    }
    int currentSource = _getPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERACTIVE);
    const char* actual = nullptr;
    switch (currentSource) {
        case DCAMPROP_TRIGGERACTIVE__EDGE:
            actual = kEdge;
            break;
        case DCAMPROP_TRIGGERACTIVE__LEVEL:
            actual = kLevel;
            break;
        default:
            throw std::runtime_error("_getSetTriggerActive() but unknown level");
            break;
    }
    CameraProperty prop(PropTriggerActive, "Trigger active");
    prop.setDiscrete(actual, { kEdge, kLevel });
    return prop;
}

CameraProperty HamamatsuCamera::_getSetTriggerPolarity(GetOrSetProperty getOrSet, const std::string& mode) {
    static const char* kPositive = "Positive";
    static const char* kNegative = "Negative";

    if (getOrSet == SetProperty) {
        if (mode == kNegative) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERPOLARITY, DCAMPROP_TRIGGERPOLARITY__NEGATIVE);
        } else if (mode == kPositive) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERPOLARITY, DCAMPROP_TRIGGERPOLARITY__POSITIVE);
        } else {
            throw std::runtime_error("unknown argument to _getSetTriggerPolarity()");
        }
    }
    int currentSource = _getPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERPOLARITY);
    const char* actual = nullptr;
    switch (currentSource) {
        case DCAMPROP_TRIGGERPOLARITY__NEGATIVE:
            actual = kNegative;
            break;
        case DCAMPROP_TRIGGERPOLARITY__POSITIVE:
            actual = kPositive;
            break;
        default:
            throw std::runtime_error("_getSetTriggerPolarity() but unknown level");
            break;
    }
    CameraProperty prop(PropTriggerPolarity, "Trigger polarity");
    prop.setDiscrete(actual, { kPositive, kNegative });
    return prop;
}

void HamamatsuCamera::_setExposureTime(const double exposureTime) {
    _setPropertyValue(_camHandle, DCAM_IDPROP_EXPOSURETIME, exposureTime);
}

double HamamatsuCamera::_getExposureTime() {
    return _getPropertyValue(_camHandle, DCAM_IDPROP_EXPOSURETIME);
}

void HamamatsuCamera::_setImageCrop(const std::pair<int, int>& crop) {
    auto sensorSize = _getSensorSize();
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHPOS, 0);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVPOS, 0);
    int rowOffset = (sensorSize.first - crop.first) / 2;
    int colOffset = (sensorSize.second - crop.second) / 2;
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHSIZE, crop.first);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVSIZE, crop.second);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHPOS, rowOffset);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVPOS, colOffset);
}
std::pair<int, int> HamamatsuCamera::_getImageCrop() {
    std::pair<int, int> cropping;
    cropping.first = _getPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHSIZE);
    cropping.second = _getPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVSIZE);
    return cropping;
}

void HamamatsuCamera::_setBinningFactor(const int binningFactor) {
    int fact = 0;
    switch (binningFactor) {
        case 1:
            fact = DCAMPROP_BINNING__1;
            break;
        case 2:
            fact = DCAMPROP_BINNING__2;
            break;
        case 4:
            fact = DCAMPROP_BINNING__4;
            break;
        default:
            throw std::runtime_error("unknown hamamatsu binning factor");
            break;
    }
    _setPropertyValue(_camHandle, DCAM_IDPROP_BINNING, fact);
}

int HamamatsuCamera::_getBinningFactor() {
    int binning = 1;
    int encoded = _getPropertyValue(_camHandle, DCAM_IDPROP_BINNING);
    switch (encoded) {
        case DCAMPROP_BINNING__1:
            binning = 1;
            break;
        case DCAMPROP_BINNING__2:
            binning = 2;
            break;
        case DCAMPROP_BINNING__4:
            binning = 4;
            break;
        default:
            throw std::runtime_error("unknown hamamatsu binning factor");
            break;
    }
    return binning;
}

void HamamatsuCamera::_setDefaults() {
    _setPropertyValue(_camHandle, DCAM_IDPROP_CCDMODE, DCAMPROP_CCDMODE__EMCCD, true);

    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__OFF);
    _sensorSize.first = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_WIDTH);
    _sensorSize.second = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_HEIGHT);

    _setPropertyValue(_camHandle, DCAM_IDPROP_READOUTSPEED, DCAMPROP_READOUTSPEED__FASTEST);

    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__ON);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHSIZE, _sensorSize.first);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVSIZE, _sensorSize.second);

    _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_POLARITY, DCAMPROP_OUTPUTTRIGGER_POLARITY__POSITIVE);
    _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__EXPOSURE, true);

    _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__INTERNAL);
    _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGER_MODE, DCAMPROP_TRIGGER_MODE__NORMAL);
    _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERACTIVE, DCAMPROP_TRIGGERACTIVE__EDGE);
    _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERPOLARITY, DCAMPROP_TRIGGERPOLARITY__NEGATIVE);
}

void HamamatsuCamera::_derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) {
    DCAMERR err;

    // try to set up a software-triggered acquisition
    if (isConfiguredForHardwareTriggering() || !_softwareTriggeredAcquisitionRunning) {
        // need to start a new acquisition, either because we can't do software triggering,
        // or because we still need to start it
        bool willDoSoftwareTriggering = !isConfiguredForHardwareTriggering();

        _initializeCamWaitHandle();
        
        err = _apiWrapper.dcambuf_alloc(_camHandle, 2);
        if (err != DCAMERR_SUCCESS) {
            throw std::runtime_error("can't allocate DCAM buffers for single acquisition");
        }
        if (willDoSoftwareTriggering) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__SOFTWARE);
        }
        err = _apiWrapper.dcamcap_start(_camHandle, DCAMCAP_START_SEQUENCE);
        if (err != DCAMERR_SUCCESS) {
            throw std::runtime_error("couldn't start snap acq");
        }
        if (willDoSoftwareTriggering) {
            _softwareTriggeredAcquisitionRunning = true;
        }
    }

    if (_softwareTriggeredAcquisitionRunning) {
        _apiWrapper.dcamcap_firetrigger(_camHandle, 0);
    }
    
    int waitMillis = std::max(1000, static_cast<int>(_getExposureTime() * 1000.0 * 2.0));
    NewImageResult result = _waitForNewImageWithTimeout(waitMillis, bufferForThisImage, nBytes);
    if (result == NoImageBeforeTimeout) {
        throw std::runtime_error("waiting for single dcam acquisition but timeout");
    }

    if (!_softwareTriggeredAcquisitionRunning) {
        _apiWrapper.dcamcap_stop(_camHandle);
        _releaseCamWaitHandle();
        _apiWrapper.dcambuf_release(_camHandle, 0);
    }
}

void HamamatsuCamera::_stopSoftwareTriggeredAcquisitionIfRunning() {
    if (_softwareTriggeredAcquisitionRunning) {
        _derivedAbortAsyncAcquisition();
        _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__INTERNAL);
        _softwareTriggeredAcquisitionRunning = false;
    }
}

void HamamatsuCamera::_derivedStartUnboundedAsyncAcquisition() {
    
    _stopSoftwareTriggeredAcquisitionIfRunning();

    DCAMERR err = _apiWrapper.dcambuf_alloc(_camHandle, kHamamatsuImagesInBuffer);
    if (err != DCAMERR_SUCCESS) {
        throw std::runtime_error("can't allocate DCAM buffers");
    }

    _initializeCamWaitHandle();

    err = _apiWrapper.dcamcap_start(_camHandle, DCAMCAP_START_SEQUENCE);
    if (err != DCAMERR_SUCCESS) {
        throw std::runtime_error("couldn't start async acq");
    }
    _numberOfImagesDelivered = 0;
}

void HamamatsuCamera::_derivedAbortAsyncAcquisition() {
    DCAMERR err = _apiWrapper.dcamcap_stop(_camHandle);
    if (err != DCAMERR_SUCCESS) {
        throw std::runtime_error("couldn't abort async acq");
    }
    _apiWrapper.dcambuf_release(_camHandle, 0);
    _releaseCamWaitHandle();
}

BaseCameraClass::NewImageResult HamamatsuCamera::_waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t *bufferForThisImage, int nBytes) {
    DCAMWAIT_START waitParams = { 0 };
    waitParams.size = sizeof(DCAMWAIT_START);
    waitParams.eventmask = DCAMWAIT_CAPEVENT_FRAMEREADY;
    waitParams.timeout = timeoutMillis;
    DCAMERR err = _apiWrapper.dcamwait_start(_camWaitHandle, &waitParams);
    if (err == DCAMERR_TIMEOUT) {
        return NoImageBeforeTimeout;
    }

    _copyLatestImage(bufferForThisImage, nBytes);

    return NewImageCopied;
}

void HamamatsuCamera::_copyLatestImage(std::uint16_t* bufferForThisImage, int nBytes) {
    auto imageSize = _getSizeOfRawImages();
    int nPixelsInImage = imageSize.first * imageSize.second;
    int nBytesPerImage = nPixelsInImage * sizeof(std::uint16_t);
    if (nBytes != nBytesPerImage) {
        throw std::runtime_error("buffer of invalid size");
    }

    DCAMERR err;
    DCAMBUF_FRAME frame;
    frame.size = sizeof(frame);
    frame.iFrame = -1;	// retrieve latest image
    frame.iKind = 0;	// reserved - must be zero or error
    frame.option = 0;	// reserved - must be zero or error
    frame.buf = bufferForThisImage;
    frame.rowbytes = imageSize.first * sizeof(std::uint16_t);
    frame.type = DCAM_PIXELTYPE_NONE;
    frame.width = imageSize.first;
    frame.height = imageSize.second;
    frame.left = 0;
    frame.top = 0;
    err = _apiWrapper.dcambuf_copyframe(_camHandle, &frame);
    if (err != DCAMERR_SUCCESS) {
        throw std::runtime_error("error in _apiWrapper.dcambuf_copyframe");
    }

    _numberOfImagesDelivered += 1;
}

void HamamatsuCamera::_initializeCamWaitHandle() {
    if (_camWaitHandle == nullptr) {
        DCAMWAIT_OPEN waitParams = { 0 };
        waitParams.size = sizeof(DCAMWAIT_OPEN);
        waitParams.hdcam = _camHandle;
        DCAMERR err = _apiWrapper.dcamwait_open(&waitParams);
        if (err != DCAMERR_SUCCESS) {
            throw std::runtime_error("couldn't get wait handle");
        }
        _camWaitHandle = waitParams.hwait;
    }
}

void HamamatsuCamera::_releaseCamWaitHandle() {
    if (_camWaitHandle != nullptr) {
        _apiWrapper.dcamwait_close(_camWaitHandle);
        _camWaitHandle = nullptr;
    }
}

std::string HamamatsuCamera::_getDCAMString(HDCAM camHandle, int stringID) {
    DCAMDEV_STRING	param = { 0 };
    char buf[512];
    param.size = sizeof(param);
    param.text = buf;
    param.textbytes = 512;
    param.iString = stringID;

    DCAMERR err = _apiWrapper.dcamdev_getstring(camHandle, &param);
    if (err != DCAMERR_SUCCESS) {
        throw std::runtime_error("error getting dcam string");
    }
    return std::string(buf);
}

bool HamamatsuCamera::_propertyIsSupported(HDCAM camHandle, int propertyID) {
    DCAMPROP_ATTR attr = { 0 };
    attr.cbSize = sizeof(DCAMPROP_ATTR);
    attr.iProp = propertyID;
    DCAMERR err = _apiWrapper.dcamprop_getattr(camHandle, &attr);
    if ((err != DCAMERR_SUCCESS) && (err != sizeof(DCAMPROP_ATTR))) {
        return false;
    }
    return (attr.attribute & DCAMPROP_ATTR_WRITABLE);
}

double HamamatsuCamera::_getPropertyValue(HDCAM camHandle, int propertyID, bool ignoreErrors) {
    double value = 0.0;
    DCAMERR err = _apiWrapper.dcamprop_getvalue(camHandle, propertyID, &value);
    if (err != DCAMERR_SUCCESS) {
        if (ignoreErrors) {
            return 0.0;
        } else {
            throw std::runtime_error("error getting dcam property value");
        }
    }
    return value;
}

void HamamatsuCamera::_setPropertyValue(HDCAM camHandle, int propertyID, double value, bool ignoreErrors) {
    DCAMERR err = _apiWrapper.dcamprop_setvalue(camHandle, propertyID, value);
    if ((err != DCAMERR_SUCCESS) && (err != DCAMERR_BUSY) && (err != DCAMERR_NOTSTABLE) && !ignoreErrors) {
        throw std::runtime_error("error setting dcam property value");
    }
}

std::pair<double, double> HamamatsuCamera::_getPropertyLimits(HDCAM camHandle, int propertyID) {
    DCAMERR err;
    DCAMPROP_ATTR attr = { 0 };
    attr.cbSize = sizeof(DCAMPROP_ATTR);
    attr.iProp = propertyID;
    err = _apiWrapper.dcamprop_getattr(camHandle, &attr);
    if ((err != DCAMERR_SUCCESS) && (err != sizeof(DCAMPROP_ATTR))) {
        throw std::runtime_error("error get dcam property limits");
    }
    return std::pair<double, double>(attr.valuemin, attr.valuemax);
}

#endif
