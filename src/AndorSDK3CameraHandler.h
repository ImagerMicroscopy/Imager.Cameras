#ifndef ANDORSDK3CAMERAHANDLER_H
#define ANDORSDK3CAMERAHANDLER_H

#include "Andor/atcore.h"

#include "ImagerPluginCore/BaseCameraClass.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenAndorSDK3Cameras();

void CloseAndorSDK3Library();

#endif
