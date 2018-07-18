#ifndef ANDORCAMERA_H
#define ANDORCAMERA_H

#include "BaseCameraClass.h"

#include "Andor/ATMCD32D.H"

class AndorCamera : public BaseCameraClass {
public:
	enum GetOrSetProperty {
		GetProperty,
		SetProperty
	};

	enum AndorPropIDs {
		PropEMGain = BaseCameraClass::FirstAvailablePropertyID,
		PropFrameTransferMode,
		PropVerticalReadoutSpeed,
		PropHorizontalReadoutSpeed,
		PropTemperatureSetPoint,
		PropCoolerOn
	};


	AndorCamera();
	~AndorCamera();

	std::string getIdentifierStr() const override;

	std::vector<CameraProperty> getCameraProperties() override;
	void setCameraProperty(const CameraProperty& prop) override;

	void setExposureTime(const double exposureTime) override;
	void setEMGain(const double emGain) override;

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	double getTemperatureSetpoint() const override { return _temperatureSetpoint; };
	std::pair<int, int> getSensorSize() const override;

private:
	AndorCapabilities _getCapabilities() const;
	bool _hasFrameTransferMode() const;
	bool _hasEMGain() const;
	CameraProperty _getSetFrameTransferMode(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetEMGain(GetOrSetProperty getOrSet, const double setPoint);
	CameraProperty _getSetHorizontalReadoutSpeeds(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetVerticalReadoutSpeeds(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetCoolerOn(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTemperatureSetPoint(GetOrSetProperty getOrSet, const double setPoint);

	void _derivedSetTemperature(const double temperature) override;
	std::pair<double, double> _derivedGetEMGainRange() override;
	void _setCoolerOn(const bool on) override;
	void _setDefaults();
	std::string _andorErrorCodeToMessage(int errorCode) const;

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
    bool _waitForNewImageWithTimeout(int timeoutMillis) override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	bool _frameTransferModeOn;
	bool _coolerOn;
	double _temperatureSetpoint;
	int _horizontalReadoutSpeedIndex;
	int _verticalReadoutSpeedIndex;
	int _numberOfImagesDelivered;
};

#endif
