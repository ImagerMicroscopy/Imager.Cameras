#ifndef HAMAMATSUCAMERAHANDLER_H
#define HAMAMATSUCAMERAHANDLER_H

#include <vector>

#include "ImagerPluginCore/BaseCameraClass.h"

std::vector<std::unique_ptr<BaseCameraClass>> OpenHamamatsuCameras();

void CloseHamamatsuLibrary();

#endif
