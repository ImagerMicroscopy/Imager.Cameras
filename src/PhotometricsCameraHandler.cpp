#include "PhotometricsCameraHandler.h"

#include "PhotometricsCamera.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenPhotometricsCameras() {
	std::vector<std::shared_ptr<BaseCameraClass>> cameras;

	int err = pl_pvcam_init();
	if (!err)
		throw std::runtime_error(PhotometricsCamera::getPVCAMErrorMessage());
	std::vector<std::string> cameraNames;
	std::int16_t nCameras;
	int pvcamErr = pl_cam_get_total(&nCameras);
	if (!pvcamErr)
		throw std::runtime_error(PhotometricsCamera::getPVCAMErrorMessage());

	for (std::int16_t i = 0; i < nCameras; i++) {
		char camName[CAM_NAME_LEN + 1];
		pvcamErr = pl_cam_get_name(i, camName);
		if (!pvcamErr)
			throw std::runtime_error(PhotometricsCamera::getPVCAMErrorMessage());
		cameraNames.push_back(std::string(camName));
	}

	for (const std::string& name : cameraNames) {
		cameras.emplace_back(new PhotometricsCamera(name));
	}

	return cameras;
}

void ClosePhotometricsLibrary() {
	pl_pvcam_uninit();
}
