#ifndef HAMAMATSUCAMERAHANDLER_H
#define HAMAMATSUCAMERAHANDLER_H

#include <vector>

#include "ImagerPluginCore/BaseCameraClass.h"
#include "ImagerPluginCore/ConfigManager.h"

std::vector<std::unique_ptr<BaseCameraClass>> OpenHamamatsuCameras(ConfigManager& configManager);

void CloseHamamatsuLibrary();

#endif
