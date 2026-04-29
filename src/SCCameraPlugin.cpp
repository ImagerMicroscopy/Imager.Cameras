#include "SCCameraPlugin.h"

#include <stdexcept>

#include "CameraManager.h"
#include "ImagerPluginCore/PluginManager.h"

static CameraManager* gCameraManager = nullptr;

void StartCameraManager(ConfigManager& configManager);
void StopCameraManager();

void InitPlugin() {
    // Imager is starting up. Create all objects and perform all work
    // needed to start operation.

    ConfigManager& configManager = PluginManager::Manager().getConfigManager();

    StartCameraManager(configManager);
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

void StartCameraManager(ConfigManager& configManager) {
    if (gCameraManager == nullptr) {
        gCameraManager = new CameraManager(configManager);
        gCameraManager->discoverCameras();
    } else {
        throw std::logic_error("CameraManager is already running");
    }
}

void StopCameraManager() {
    if (gCameraManager != nullptr) {
        delete gCameraManager;
        gCameraManager = nullptr;
    }
}
