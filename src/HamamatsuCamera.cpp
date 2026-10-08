#include "HamamatsuCamera.h"

#include "Hamamatsu/dcamprop.h"

#include "ImagerPluginCore/StandardCameraProperties.h"

#include "SCPrinter.h"

HamamatsuCamera::HamamatsuCamera(HDCAM camHandle, ConfigManager& configManager) :
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

    _loadEnabledProperties(configManager);
    _setDefaults();
    _applyConfiguredDefaultValues(configManager);
}

namespace {
    struct ConfigurableProperty {
        int propertyCode;
        const char* configKey;
        bool enabledByDefault;
        bool isNumeric;
    };

    // Hamamatsu-specific properties that can be disabled or given a default value in the config file,
    // under [<camera name>.EnabledProperties] and [<camera name>.DefaultValues].
    const std::vector<ConfigurableProperty>& ConfigurableProperties() {
        static const std::vector<ConfigurableProperty> kProperties = {
            { HamamatsuCamera::PropEMMode, "EMMode", true, false },
            { HamamatsuCamera::PropEMGain, "EMGain", true, true },
            { HamamatsuCamera::PropReadoutSpeed, "ReadoutSpeed", true, false },
            { HamamatsuCamera::PropTemperatureSetPoint, "TemperatureSetPoint", true, true },
            { HamamatsuCamera::PropCoolerOn, "CoolerOn", true, false },
            { HamamatsuCamera::PropTriggerSource, "TriggerSource", true, false },
            { HamamatsuCamera::PropTriggerMode, "TriggerMode", true, false },
            { HamamatsuCamera::PropTriggerActive, "TriggerActive", true, false },
            { HamamatsuCamera::PropTriggerPolarity, "TriggerPolarity", true, false },
            { HamamatsuCamera::PropOutputTriggerKind, "OutputTriggerKind", false, false },
            { HamamatsuCamera::PropOutputTriggerPeriod, "OutputTriggerPeriod", false, true },
            { HamamatsuCamera::PropOutputTriggerPolarity, "OutputTriggerPolarity", false, false }
        };
        return kProperties;
    }
}

void HamamatsuCamera::_loadEnabledProperties(ConfigManager& configManager) {
    // Disabled properties are never exposed, set from the GUI, or given a built-in default value,
    // as if the plugin did not know about them. Missing entries are written to the config file.
    _disabledProperties.clear();
    for (const auto& p : ConfigurableProperties()) {
        auto setting = configManager.getBoolSettingOrDefault(_camName / "EnabledProperties" / p.configKey, p.enabledByDefault);
        if (!setting.value) {
            _disabledProperties.insert(p.propertyCode);
        }
    }
}

void HamamatsuCamera::_applyConfiguredDefaultValues(ConfigManager& configManager) {
    // An empty value means "no default". Discrete properties take the option name as shown in the GUI
    // (e.g. "Programmable"), numeric properties a number (e.g. "0.001").
    for (const auto& p : ConfigurableProperties()) {
        auto setting = configManager.getStringSettingOrDefault(_camName / "DefaultValues" / p.configKey, "");
        if (setting.value.empty()) {
            continue;
        }
        try {
            double value = p.isNumeric ? std::stod(setting.value) : 0.0;
            _setHamamatsuProperty(p.propertyCode, setting.value, value);
        }
        catch (const std::exception& e) {
            Print(_camName + ": ignoring default value '" + setting.value + "' for " + p.configKey + ": " + e.what());
        }
    }
}

HamamatsuCamera::~HamamatsuCamera() {
    if (_camHandle != nullptr) {
        _apiWrapper.dcamdev_close(_camHandle);
        _camHandle = nullptr;
    }
}

std::string HamamatsuCamera::getIdentifierStr() {
    return _camName;
}

double HamamatsuCamera::getFrameRate() {
    return _getPropertyValue(_camHandle, DCAM_IDPROP_INTERNALFRAMERATE);
}

std::vector<CameraProperty> HamamatsuCamera::_derivedGetCameraProperties() {
    std::vector<CameraProperty> properties;
    properties = GetStandardProperties(_getExposureTime(), _getImageCrop(), StandardCroppingOptions(_getSensorSize().first),
                                       StandardCroppingOptions(_getSensorSize().second), _getBinningFactor(), { 1, 2, 4 });

    if (_isPropertyEnabled(PropEMMode) && _propertyIsSupported(_camHandle, DCAM_IDPROP_CCDMODE)) {
        properties.push_back(_getSetEMMode(GetProperty, std::string()));
    }
    if (_isPropertyEnabled(PropEMGain) && _propertyIsSupported(_camHandle, DCAM_IDPROP_SENSITIVITY)) {
        properties.push_back(_getSetEMGain(GetProperty, 0.0));
    }
    if (_isPropertyEnabled(PropReadoutSpeed) && _propertyIsSupported(_camHandle, DCAM_IDPROP_READOUTSPEED)) {
        properties.push_back(_getSetReadoutSpeed(GetProperty, std::string()));
    }
    if (_isPropertyEnabled(PropCoolerOn) && _propertyIsSupported(_camHandle, DCAM_IDPROP_SENSORCOOLER)) {
        properties.push_back(_getSetCoolerOn(GetProperty, std::string()));
    }
    if (_isPropertyEnabled(PropTemperatureSetPoint) && _propertyIsSupported(_camHandle, DCAM_IDPROP_SENSORTEMPERATURETARGET)) {
        properties.push_back(_getSetTemperatureSetPoint(GetProperty, 0.0));
    }
    if (_isPropertyEnabled(PropTriggerSource) && _propertyIsSupported(_camHandle, DCAM_IDPROP_TRIGGERSOURCE)) {
        properties.push_back(_getSetTriggerSource(GetProperty, std::string()));
    }
    if (_isPropertyEnabled(PropTriggerMode) && _propertyIsSupported(_camHandle, DCAM_IDPROP_TRIGGER_MODE)) {
        properties.push_back(_getSetTriggerMode(GetProperty, std::string()));
    }
    if (_isPropertyEnabled(PropOutputTriggerKind) && _propertyIsSupported(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND)) {
        properties.push_back(_getSetOutputTriggerKind(GetProperty, std::string()));
    }
    if (_isPropertyEnabled(PropOutputTriggerPeriod) && _propertyIsSupported(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_PERIOD)) {
        properties.push_back(_getSetOutputTriggerPeriod(GetProperty, 0.0001));
    }
    if (_isPropertyEnabled(PropOutputTriggerPolarity) && _propertyIsSupported(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_POLARITY)) {
        properties.push_back(_getSetOutputTriggerPolarity(GetProperty, std::string()));
    }
    if (_isPropertyEnabled(PropTriggerActive) && _propertyIsSupported(_camHandle, DCAM_IDPROP_TRIGGERACTIVE)) {
        properties.push_back(_getSetTriggerActive(GetProperty, std::string()));
    }
    if (_isPropertyEnabled(PropTriggerPolarity) && _propertyIsSupported(_camHandle, DCAM_IDPROP_TRIGGERPOLARITY)) {
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
        if (!_isPropertyEnabled(prop.getPropertyCode())) {
            // disabled in the config file, e.g. a stale value from previously saved settings
            continue;
        }
        bool isNumeric = (prop.getPropertyCode() == PropEMGain) || (prop.getPropertyCode() == PropTemperatureSetPoint) ||
                         (prop.getPropertyCode() == PropOutputTriggerPeriod);
        _setHamamatsuProperty(prop.getPropertyCode(), isNumeric ? std::string() : prop.getCurrentOption(),
                              isNumeric ? prop.getValue() : 0.0);
    }
}

void HamamatsuCamera::_setHamamatsuProperty(int propertyCode, const std::string& option, double value) {
    switch (propertyCode) {
        case PropEMMode:
            _getSetEMMode(SetProperty, option);
            break;
        case PropEMGain:
            _getSetEMGain(SetProperty, value);
            break;
        case PropReadoutSpeed:
            _getSetReadoutSpeed(SetProperty, option);
            break;
        case PropTemperatureSetPoint:
            _getSetTemperatureSetPoint(SetProperty, value);
            break;
        case PropCoolerOn:
            _getSetCoolerOn(SetProperty, option);
            break;
        case PropTriggerSource:
            _getSetTriggerSource(SetProperty, option);
            break;
        case PropTriggerMode:
            _getSetTriggerMode(SetProperty, option);
            break;
        case PropTriggerActive:
            _getSetTriggerActive(SetProperty, option);
            break;
        case PropTriggerPolarity:
            _getSetTriggerPolarity(SetProperty, option);
            break;
        case PropOutputTriggerKind:
            _getSetOutputTriggerKind(SetProperty, option);
            break;
        case PropOutputTriggerPolarity:
            _getSetOutputTriggerPolarity(SetProperty, option);
            break;
        case PropOutputTriggerPeriod:
            _getSetOutputTriggerPeriod(SetProperty, value);
            break;
        default:
            throw std::runtime_error("setting unrecognized option");
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
    CameraProperty prop(PropTriggerSource, "Trigger source [IN]");
    prop.setDiscrete(actual, { kInternal, kExternal });
    return prop;
}


CameraProperty HamamatsuCamera::_getSetOutputTriggerPeriod(GetOrSetProperty getOrSet, const double value)
{
    if (getOrSet == SetProperty) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_PERIOD, value, true);
    }
    CameraProperty prop(PropOutputTriggerPeriod, "Trigger period [OUT]");
    prop.setNumeric(_getPropertyValue(_camHandle,  DCAM_IDPROP_OUTPUTTRIGGER_PERIOD));
    return prop;
}


CameraProperty HamamatsuCamera::_getSetOutputTriggerKind(GetOrSetProperty getOrSet, const std::string& mode)
{
    static const char* programmable = "Programmable";
    static const char* low          = "Low";
    static const char* globalexp    = "Global Exposure";
    static const char* triggerready = "Trigger Ready";
    static const char* high         = "High";
    static const char* anyrowexp    = "Any Row Exposure";
    if (getOrSet == SetProperty) {

        if (mode == programmable) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__PROGRAMABLE);
        } else if (mode == low) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__LOW);
        } else if (mode == globalexp) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__GLOBALEXPOSURE);
        } else if (mode == triggerready) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__TRIGGERREADY);
        } else if (mode == high) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__HIGH);
        } else if (mode == anyrowexp) {
            _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__ANYROWEXPOSURE);       
        }
        else {
            throw std::runtime_error("unknown argument to _getSetOutputTriggerKind()");
        }
    }
    int currentSource = _getPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND);
    const char* actual = nullptr;
    switch (currentSource) {
        case DCAMPROP_OUTPUTTRIGGER_KIND__PROGRAMABLE:
            actual = programmable;
            break;
        case DCAMPROP_OUTPUTTRIGGER_KIND__LOW:
            actual = low;
            break;
        case DCAMPROP_OUTPUTTRIGGER_KIND__GLOBALEXPOSURE:
            actual = globalexp;
            break;
        case DCAMPROP_OUTPUTTRIGGER_KIND__TRIGGERREADY:
            actual = triggerready;
            break;
        case DCAMPROP_OUTPUTTRIGGER_KIND__HIGH:
            actual = high;
            break;
        case DCAMPROP_OUTPUTTRIGGER_KIND__ANYROWEXPOSURE:
            actual = anyrowexp;
            break;

        default:
            throw std::runtime_error("_getSetOutputTriggerKind() but unknown mode");
            break;
    }   
    CameraProperty prop(PropOutputTriggerKind, "Trigger kind [OUT]");
    prop.setDiscrete(actual, {programmable, low, globalexp, triggerready, high, anyrowexp });
    return prop;
}

CameraProperty HamamatsuCamera::_getSetOutputTriggerPolarity(GetOrSetProperty getOrSet,
                                                            const std::string& mode)
{
    static const char* negative = "Negative";
    static const char* positive = "Positive";

    if (getOrSet == SetProperty) {

        if (mode == negative) {
            _setPropertyValue(_camHandle,
                              DCAM_IDPROP_OUTPUTTRIGGER_POLARITY,
                              DCAMPROP_OUTPUTTRIGGER_POLARITY__NEGATIVE);
        } else if (mode == positive) {
            _setPropertyValue(_camHandle,
                              DCAM_IDPROP_OUTPUTTRIGGER_POLARITY,
                              DCAMPROP_OUTPUTTRIGGER_POLARITY__POSITIVE);
        } else {
            throw std::runtime_error("unknown argument to _getSetOutputTriggerPolarity()");
        }
    }

    int currentPolarity =
        _getPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_POLARITY);

    const char* actual = nullptr;
    switch (currentPolarity) {
        case DCAMPROP_OUTPUTTRIGGER_POLARITY__NEGATIVE:
            actual = negative;
            break;
        case DCAMPROP_OUTPUTTRIGGER_POLARITY__POSITIVE:
            actual = positive;
            break;
        default:
            throw std::runtime_error("_getSetOutputTriggerPolarity() but unknown polarity");
    }

    CameraProperty prop(PropOutputTriggerPolarity, "Trigger polarity [OUT]");
    prop.setDiscrete(actual, { negative, positive });
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
    CameraProperty prop(PropTriggerMode, "Trigger mode [IN]");
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
    CameraProperty prop(PropTriggerActive, "Trigger active [IN]");
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
    CameraProperty prop(PropTriggerPolarity, "Trigger polarity [IN]");
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
    if (_isPropertyEnabled(PropEMMode)) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_CCDMODE, DCAMPROP_CCDMODE__EMCCD, true);
    }

    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__OFF);
    _sensorSize.first = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_WIDTH);
    _sensorSize.second = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_HEIGHT);

    if (_isPropertyEnabled(PropReadoutSpeed)) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_READOUTSPEED, DCAMPROP_READOUTSPEED__FASTEST);
    }

    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__ON);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHSIZE, _sensorSize.first);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVSIZE, _sensorSize.second);

    if (_isPropertyEnabled(PropOutputTriggerPolarity)) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_POLARITY, DCAMPROP_OUTPUTTRIGGER_POLARITY__POSITIVE);
    }
    if (_isPropertyEnabled(PropTriggerSource)) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__INTERNAL);
    }
    if (_isPropertyEnabled(PropTriggerMode)) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGER_MODE, DCAMPROP_TRIGGER_MODE__NORMAL);
    }
    if (_isPropertyEnabled(PropTriggerActive)) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERACTIVE, DCAMPROP_TRIGGERACTIVE__EDGE);
    }
    if (_isPropertyEnabled(PropTriggerPolarity)) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERPOLARITY, DCAMPROP_TRIGGERPOLARITY__POSITIVE);
    }
    if (_isPropertyEnabled(PropOutputTriggerKind)) {
        _setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__PROGRAMABLE, true);
    }
}

AcquiredImage HamamatsuCamera::_derivedAcquireSingleImage() {
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
    
    int waitMillis = std::max(5000, static_cast<int>(_getExposureTime() * 1000.0 * 2.0));
    auto acquiredImage = _waitForNewImageWithTimeout(waitMillis);
    if (!acquiredImage.has_value()) {
        throw std::runtime_error("waiting for single dcam acquisition but timeout");
    }

    if (!_softwareTriggeredAcquisitionRunning) {
        _apiWrapper.dcamcap_stop(_camHandle);
        _releaseCamWaitHandle();
        _apiWrapper.dcambuf_release(_camHandle, 0);
    }

    return std::move(acquiredImage.value());
}

void HamamatsuCamera::_stopSoftwareTriggeredAcquisitionIfRunning() {
    if (_softwareTriggeredAcquisitionRunning) {
        _derivedAbortAsyncAcquisition();
        _setPropertyValue(_camHandle, DCAM_IDPROP_TRIGGERSOURCE, DCAMPROP_TRIGGERSOURCE__INTERNAL);
        _softwareTriggeredAcquisitionRunning = false;
    }
}

void HamamatsuCamera::_derivedStartBoundedAsyncAcquisition([[maybe_unused]] std::uint64_t nImagesToAcquire) {
    // We ignore nImagesToAcquire - the caller will abort the acquisition anyway. However,
    // it's possible that we will acquire more images than needed - these will be discarded.
    
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

std::optional<AcquiredImage> HamamatsuCamera::_waitForNewImageWithTimeout(int timeoutMillis) {
    DCAMWAIT_START waitParams = { 0 };
    waitParams.size = sizeof(DCAMWAIT_START);
    waitParams.eventmask = DCAMWAIT_CAPEVENT_FRAMEREADY;
    waitParams.timeout = timeoutMillis;
    DCAMERR err = _apiWrapper.dcamwait_start(_camWaitHandle, &waitParams);
    if (err == DCAMERR_TIMEOUT) {
        return std::nullopt;
    }

    auto actualImageSize = _getSizeOfRawImages();
    AcquiredImage image = NewRecycledImage(AcquiredImage::PixelFormat::Mono16, actualImageSize.first, actualImageSize.second);
    _copyLatestImage(reinterpret_cast<std::uint16_t*>(image.getData().get()), image.getNRows() * image.getNCols() * sizeof(std::uint16_t));

    return image;
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

