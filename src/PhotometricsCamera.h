
#include "BaseCameraClass.h"

class PhotometricsCamera : public BaseCameraClass {
public:
	PhotometricsCamera(const std::string& cameraName);
	~PhotometricsCamera();

	bool setExposureTime(const double exposureTime) override;
	bool setEMGain(const double emGain) override;
	bool setTemperature(const double temperature) override;

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	std::pair<int, int> getSensorSize() const override;

	std::vector<uint16_t> PhotometricsCamera::acquireImages(const int nImages) override;

private:
	std::int16_t _pvcamHandle;
	double _requestedExposureTime;
};
