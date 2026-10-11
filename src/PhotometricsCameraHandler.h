#ifndef PHOTOMETRICSCAMERAHANDLER_H
#define PHOTOMETRICSCAMERAHANDLER_H

#include <vector>

#include "ImagerPluginCore/BaseCameraClass.h"
#include "ImagerPluginCore/ConfigManager.h"

std::vector<std::unique_ptr<BaseCameraClass>> OpenPhotometricsCameras(ConfigManager& configManager);

void ClosePhotometricsLibrary();

#endif