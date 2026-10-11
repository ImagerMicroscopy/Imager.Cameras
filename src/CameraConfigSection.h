#ifndef CAMERACONFIGSECTION_H
#define CAMERACONFIGSECTION_H

#include <filesystem>
#include <string>

#include "ImagerPluginCore/ConfigManager.h"

// Gives a camera access to its own section of the plugin configuration file.
// All keys are implicitly prefixed with the camera's identifier, so a camera
// can only access its own settings.
class CameraConfigSection {
public:
    CameraConfigSection(ConfigManager& configManager, const std::string& cameraIdentifier);

    ConfigManager::ConfigSetting<std::string> getStringSettingOrDefault(const std::string& key, const std::string& defaultValue) const;
    ConfigManager::ConfigSetting<bool> getBoolSettingOrDefault(const std::string& key, bool defaultValue) const;
    ConfigManager::ConfigSetting<int> getIntSettingOrDefault(const std::string& key, int defaultValue) const;
    ConfigManager::ConfigSetting<double> getDoubleSettingOrDefault(const std::string& key, double defaultValue) const;
    ConfigManager::ConfigSetting<std::filesystem::path> getPathSettingOrDefault(const std::string& key, const std::filesystem::path& defaultValue) const;

private:
    ConfigManager* _configManager;
    std::string _cameraIdentifier;
};

#endif // CAMERACONFIGSECTION_H
