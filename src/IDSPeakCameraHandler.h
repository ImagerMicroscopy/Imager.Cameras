#ifndef IDSPEAKCAMERAHANDLER_H
#define IDSPEAKCAMERAHANDLER_H

#include "ids_peak_comfort_c/ids_peak_comfort_c.h"

#include "BaseCameraClass.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenIDSPeakCameras();

void CloseIDSPeakLibrary();

#endif
