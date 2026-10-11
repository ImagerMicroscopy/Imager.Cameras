#ifndef SCCAMERABASE_H
#define SCCAMERABASE_H

#include <memory>
#include <string>

#include "ImagerPluginCore/BaseCameraClass.h"
#include "ImagerPluginCore/ConfigManager.h"

#include "CameraConfigSection.h"

// Base class for all cameras of this plugin. Each camera gets access to its
// own section of the plugin configuration file and applies its settings at
// startup: the image orientation options, followed by camera-specific settings.
class SCCameraBase : public BaseCameraClass {
public:
    SCCameraBase(ConfigManager& configManager);
    virtual ~SCCameraBase() = default;

protected:
    // Applies the settings from this camera's own configuration file section.
    // Must be called as the last step of the concrete camera's constructor:
    // virtual dispatch of _derivedApplyConfigFileSettings() does not work when
    // this is called from the SCCameraBase constructor.
    void applyConfigFileSettings();

    // Hook for camera-specific settings from the configuration file. Called by
    // applyConfigFileSettings() after the orientation options were applied.
    virtual void _derivedApplyConfigFileSettings() {}

    // This camera's own section of the plugin configuration file.
    CameraConfigSection& configSection();

private:
    ConfigManager* _configManager;
    std::unique_ptr<CameraConfigSection> _configSection;
};

#endif // SCCAMERABASE_H
