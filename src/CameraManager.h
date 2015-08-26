#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <memory>

class BaseCameraClass;

class CameraManager {
public:
	CameraManager();
	~CameraManager();

	void discoverCameras();
	
	std::vector<std::string> getCameraIdentifiers() const;
	
	std::shared_ptr<BaseCameraClass> getCamera(const std::string& identifier) const;
	std::shared_ptr<BaseCameraClass> getFirstCamera() const;

private:
	std::map<std::string, std::shared_ptr<BaseCameraClass>> _availableCameras;
};

#endif
