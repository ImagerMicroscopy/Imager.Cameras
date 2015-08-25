
#include "BaseCameraClass.h"

class PhotometricsCamera : public BaseCameraClass {
public:
	PhotometricsCamera(const std::string& cameraName);
	~PhotometricsCamera();

	bool setExposureTime(const double exposureTime) override;
	bool setEMGain(const double emGain) override;


	std::vector<uint16_t> PhotometricsCamera::acquireImages(const int nImages) override;

private:
	int _pvcamHandle;
	double _requestedExposureTime;
};
