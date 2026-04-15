#include "SCCameraPlugin.h"

#include "PluginManager.h"

void InitPlugin() {
    // Imager is starting up. Create all objects and perform all work
    // needed to start operation.

    // the Manager object needs to know about all equipment provided by the plugin
    auto& manager = PluginManager::Manager();
}

void ShutdownPlugin() {
    // Imager is closing.
    // Perform any necessary cleanup here.
}
