#ifndef PHOTOMETRICSCAMERA_H
#define PHOTOMETRICSCAMERA_H

#include "BaseCameraClass.h"

class PhotometricsCamera : public BaseCameraClass {
public:
	PhotometricsCamera(const std::string& cameraName);
	~PhotometricsCamera();

	std::string getIdentifierStr() const override;

	bool setExposureTime(const double exposureTime) override;
	bool setEMGain(const double emGain) override;
	bool setCoolerOnOff(const bool on) { return false; };
	bool setTemperature(const double temperature) override;

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	std::pair<int, int> getSensorSize() const override;

	std::vector<uint16_t> acquireImages(const int nImages) override;

	static std::string getPVCAMErrorMessage();

private:
	void _selectFastestReadoutPort(bool useEMGain);
	void _validateExposureTime();

	std::string _identifier;
	std::int16_t _pvcamHandle;
	double _requestedExposureTime;
};

#endif
