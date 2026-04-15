#ifndef PHOTOMETRICSCAMERAHANDLER_H
#define PHOTOMETRICSCAMERAHANDLER_H

#include <vector>

#include "ImagerPluginCore/BaseCameraClass.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenPhotometricsCameras();

void ClosePhotometricsLibrary();

#endif