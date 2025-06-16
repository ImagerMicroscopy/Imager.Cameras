#ifndef IDSPEAKCAMERAHANDLER_H
#define IDSPEAKCAMERAHANDLER_H

#include "BaseCameraClass.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenIDSPeakCameras();

void CloseIDSPeakLibrary();

#endif
