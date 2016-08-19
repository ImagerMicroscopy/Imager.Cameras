#ifndef PHOTOMETRICSCAMERA_H
#define PHOTOMETRICSCAMERA_H

#include "BaseCameraClass.h"

class PhotometricsCamera : public BaseCameraClass {
public:
	PhotometricsCamera(const std::string& cameraName);
	~PhotometricsCamera();

	std::string getIdentifierStr() const override;

	void setExposureTime(const double exposureTime) override;
	void setEMGain(const double emGain) override;
	bool setTemperature(const double temperature) override;

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	double getTemperatureSetpoint() const override;
	std::pair<int, int> getSensorSize() const override;

	static std::string getPVCAMErrorMessage();

private:
	void _setCoolerOn(const bool on) override { ; };
	void _selectFastestReadoutPort(bool useEMGain);
	void _validateExposureTime();

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	std::string _identifier;
	std::int16_t _pvcamHandle;
	double _requestedExposureTime;
	std::vector<std::uint16_t> _asyncBuffer;
};

#endif
