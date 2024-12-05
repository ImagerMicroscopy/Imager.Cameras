#include "IDSPeakCamera.h"

#include <stdexcept>

IDSPeakCamera::IDSPeakCamera(peak_camera_handle camHandle) :
    _camHandle(camHandle)
{
    peak_status status = PEAK_STATUS_SUCCESS;
    peak_Camera_ResetToDefaultSettings(_camHandle);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("couldn't reset IDS Peak camera to default settings");
    }

    
}