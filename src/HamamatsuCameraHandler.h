#ifndef HAMAMATSUCAMERAHANDLER_H
#define HAMAMATSUCAMERAHANDLER_H

#include <vector>

#include "BaseCameraClass.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenHamamatsuCameras();

void CloseHamamatsuLibrary();

#endif
