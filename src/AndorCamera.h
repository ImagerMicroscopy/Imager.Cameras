#ifndef ANDORCAMERA_H
#define ANDORCAMERA_H

#include "BaseCameraClass.h"

class AndorCamera : public BaseCameraClass {
public:
	AndorCamera();
	~AndorCamera();

	std::string getIdentifierStr() const override;

	void setExposureTime(const double exposureTime) override;
	void setEMGain(const double emGain) override;
	bool setCoolerOn(const bool on);
	bool setTemperature(const double temperature) override;

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	double getTemperatureSetpoint() const override { return _temperatureSetpoint; };
	std::pair<int, int> getSensorSize() const override;

private:
	void _setDefaults();
	void _selectFastestRecommendedReadoutSpeed();
	std::string _andorErrorCodeToMessage(int errorCode) const;

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	bool _coolerOn;
	double _temperatureSetpoint;
	int _numberOfImagesAcquired;
};

#endif
