
#include "CameraManager.h"

#ifdef WITH_PHOTOMETRICS
#include "PVCAM/master.h"
#include "PVCAM/pvcam.h"
#include "PhotometricsCamera.h"
#endif

#ifdef WITH_ANDOR
#include "Andor/ATMCD32D.H"
#include "AndorCamera.h"
#endif

CameraManager::CameraManager() {
#ifdef WITH_PHOTOMETRICS
	pl_pvcam_init();
#endif
}

CameraManager::~CameraManager() {
#ifdef WITH_PHOTOMETRICS
	pl_pvcam_uninit();
#endif
}

void CameraManager::discoverCameras() {
	_availableCameras.clear();

#ifdef WITH_PHOTOMETRICS
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
#endif

#ifdef WITH_ANDOR
	try {
		std::shared_ptr<BaseCameraClass> andorCamera(new AndorCamera());
		std::string identifier = andorCamera->getIdentifierStr();
		_availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(identifier, andorCamera));
	}
	catch (std::runtime_error e) {
		// no Andor camera available
	}
#endif
}

std::vector<std::string> CameraManager::getCameraIdentifiers() const {
	std::vector<std::string> identifiers;

	for (auto it = _availableCameras.cbegin(); it != _availableCameras.cend(); it++) {
		identifiers.push_back(it->first);
	}

	return identifiers;
}

std::shared_ptr<BaseCameraClass> CameraManager::getCamera(const std::string& identifier) const {
	if (_availableCameras.count(identifier) == 0)
		throw std::runtime_error("Camera not found");
	return _availableCameras.at(identifier);
}

std::shared_ptr<BaseCameraClass> CameraManager::getFirstCamera() const {
	if (_availableCameras.size() == 0)
		throw std::runtime_error("No cameras found");
	return _availableCameras.begin()->second;
}
