#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <exception>

#include "ImagerPluginCore/ConfigManager.h"

#include "SCConfigurationFile.h"

class BaseCameraClass;

class CameraManager {
public:
    CameraManager(ConfigManager& configManager) : _configManager(&configManager) {}
    ~CameraManager();

    void discoverCameras();
    
    std::vector<std::string> getCameraIdentifiers() const;
    
    std::shared_ptr<BaseCameraClass> getCamera(const std::string& identifier) const;
    std::shared_ptr<BaseCameraClass> getFirstCamera() const;

    void abortRunningAcquisitions();

private:
    void _applyCameraOrientationOptions();

    ConfigManager* _configManager = nullptr;
    std::map<std::string, std::shared_ptr<BaseCameraClass>> _availableCameras;
};

#endif
