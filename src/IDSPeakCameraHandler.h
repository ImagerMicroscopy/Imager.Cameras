#ifndef IDSPEAKCAMERAHANDLER_H
#define IDSPEAKCAMERAHANDLER_H

#include "ImagerPluginCore/BaseCameraClass.h"
#include "ImagerPluginCore/ConfigManager.h"

std::vector<std::unique_ptr<BaseCameraClass>> OpenIDSPeakCameras(ConfigManager& configManager);

void CloseIDSPeakLibrary();

#endif
