#include "SCCameraBase.h"

#include "SCConfigurationFile.h"

SCCameraBase::SCCameraBase(ConfigManager& configManager) :
    _configManager(&configManager)
{
}

void SCCameraBase::applyConfigFileSettings() {
    std::string orientationStr = configSection().getStringSettingOrDefault("orientation", "").value;
    setImageProcessingOps(ParseOrientationOptions(orientationStr));
    _derivedApplyConfigFileSettings();
}

CameraConfigSection& SCCameraBase::configSection() {
    if (!_configSection) {
        _configSection = std::make_unique<CameraConfigSection>(*_configManager, getIdentifierStr());
    }
    return *_configSection;
}
