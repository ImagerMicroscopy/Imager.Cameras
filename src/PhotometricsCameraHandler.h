#ifndef PHOTOMETRICSCAMERAHANDLER_H
#define PHOTOMETRICSCAMERAHANDLER_H

#include <vector>

#include "BaseCameraClass.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenPhotometricsCameras();

void ClosePhotometricsLibrary();

#endif