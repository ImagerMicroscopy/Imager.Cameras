
#include "CameraManager.h"

#include "PVCAM/master.h"
#include "PVCAM/pvcam.h"
#include "Andor/ATMCD32D.H"

#include "PhotometricsCamera.h"
#include "AndorCamera.h"

CameraManager::CameraManager() {
	pl_pvcam_init();
}

CameraManager::~CameraManager() {
	pl_pvcam_uninit();
}

void CameraManager::discoverCameras() {
	_availableCameras.clear();

	// Roper cameras
	std::vector<std::string> cameraNames;
	std::int16_t nCameras;
	int err = pl_cam_get_total(&nCameras);
	for (std::int16_t i = 0; i < nCameras; i++) {
		char camName[CAM_NAME_LEN + 1];
		pl_cam_get_name(i, camName);
		cameraNames.push_back(std::string(camName));
	}

	for (const std::string& name : cameraNames) {
		std::shared_ptr<BaseCameraClass> newCamera(new PhotometricsCamera(name));
		_availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(name, newCamera));
	}

	// Andor camera
	try {
		std::shared_ptr<BaseCameraClass> andorCamera(new AndorCamera());
		std::string identifier = andorCamera->getIdentifierStr();
		_availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(identifier, andorCamera));
	}
	catch (std::runtime_error e) {
		// no Andor camera available
	}
}

std::vector<std::string> CameraManager::getCameraIdentifiers() const {
	std::vector<std::string> identifiers;

	for (auto it = _availableCameras.cbegin(); it != _availableCameras.cend(); it++) {
		identifiers.push_back(it->first);
	}

	return identifiers;
}

std::shared_ptr<BaseCameraClass> CameraManager::getCamera(const std::string& identifier) {
	return _availableCameras.at(identifier);
}
