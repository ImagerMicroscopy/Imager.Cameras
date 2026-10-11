#include "CameraConfigSection.h"

CameraConfigSection::CameraConfigSection(ConfigManager& configManager, const std::string& cameraIdentifier) :
    _configManager(&configManager),
    _cameraIdentifier(cameraIdentifier)
{
}

ConfigManager::ConfigSetting<std::string> CameraConfigSection::getStringSettingOrDefault(const std::string& key, const std::string& defaultValue) const {
    return _configManager->getStringSettingOrDefault(_cameraIdentifier / key, defaultValue);
}

ConfigManager::ConfigSetting<bool> CameraConfigSection::getBoolSettingOrDefault(const std::string& key, bool defaultValue) const {
    return _configManager->getBoolSettingOrDefault(_cameraIdentifier / key, defaultValue);
}

ConfigManager::ConfigSetting<int> CameraConfigSection::getIntSettingOrDefault(const std::string& key, int defaultValue) const {
    return _configManager->getIntSettingOrDefault(_cameraIdentifier / key, defaultValue);
}

ConfigManager::ConfigSetting<double> CameraConfigSection::getDoubleSettingOrDefault(const std::string& key, double defaultValue) const {
    return _configManager->getDoubleSettingOrDefault(_cameraIdentifier / key, defaultValue);
}

ConfigManager::ConfigSetting<std::filesystem::path> CameraConfigSection::getPathSettingOrDefault(const std::string& key, const std::filesystem::path& defaultValue) const {
    return _configManager->getPathSettingOrDefault(_cameraIdentifier / key, defaultValue);
}
