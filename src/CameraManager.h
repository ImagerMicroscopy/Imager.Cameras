#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <exception>

#include "SCConfigurationFile.h"

class BaseCameraClass;

class CameraManager {
public:
    CameraManager(const SCConfigurationFile& configFile) : _configFile(configFile) {}
    ~CameraManager();

    void discoverCameras();
    
    std::vector<std::string> getCameraIdentifiers() const;
    
    std::shared_ptr<BaseCameraClass> getCamera(const std::string& identifier) const;
    std::shared_ptr<BaseCameraClass> getFirstCamera() const;

    void abortRunningAcquisitions();

private:
    void _applyCameraOrientationOptions();

    std::map<std::string, std::shared_ptr<BaseCameraClass>> _availableCameras;
    SCConfigurationFile _configFile;
};

#endif
