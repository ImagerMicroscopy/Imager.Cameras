#include "SCCameraPlugin.h"

#include <stdexcept>

#include "CameraManager.h"
#include "ImagerPluginCore/PluginManager.h"

static CameraManager* gCameraManager = nullptr;

void StartCameraManager(const fs::path& configDirPath);
void StopCameraManager();

void InitPlugin(const std::filesystem::path& configDirPath) {
    // Imager is starting up. Create all objects and perform all work
    // needed to start operation.

    StartCameraManager(configDirPath);
    gCameraManager->discoverCameras();
    std::vector<std::string> cameraNames = gCameraManager->getCameraIdentifiers();

    for (const auto& name : cameraNames) {
        PluginManager::Manager().addCamera(gCameraManager->getCamera(name));
    }
}

void ShutdownPlugin() {
    // Imager is closing.
    // Perform any necessary cleanup here.
    StopCameraManager();
}

void StartCameraManager(const fs::path& configDirPath) {
    if (gCameraManager == nullptr) {
        SCConfigurationFile configFile(configDirPath);
        gCameraManager = new CameraManager(configFile);
        gCameraManager->discoverCameras();
    }
}

void StopCameraManager() {
    if (gCameraManager != nullptr) {
        delete gCameraManager;
        gCameraManager = nullptr;
    }
}
