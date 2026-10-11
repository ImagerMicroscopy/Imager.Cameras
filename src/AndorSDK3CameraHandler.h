#ifndef ANDORSDK3CAMERAHANDLER_H
#define ANDORSDK3CAMERAHANDLER_H

#include "Andor/atcore.h"

#include "ImagerPluginCore/BaseCameraClass.h"
#include "ImagerPluginCore/ConfigManager.h"

std::vector<std::unique_ptr<BaseCameraClass>> OpenAndorSDK3Cameras(ConfigManager& configManager);

void CloseAndorSDK3Library();

#endif
