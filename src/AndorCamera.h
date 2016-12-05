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

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	double getTemperatureSetpoint() const override { return _temperatureSetpoint; };
	std::pair<int, int> getSensorSize() const override;

private:
	void _derivedSetTemperature(const double temperature) override;
	std::pair<double, double> _derivedGetEMGainRange() override;
	void _setCoolerOn(const bool on) override;
	void _setDefaults();
	void _selectFastestRecommendedReadoutSpeed();
	std::string _andorErrorCodeToMessage(int errorCode) const;

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	bool _coolerOn;
	double _temperatureSetpoint;
	int _numberOfImagesDelivered;
};

#endif
