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
		PropEMGain = CameraProperty::FirstAvailablePropertyID,
		PropFrameTransferMode,
		PropVerticalReadoutSpeed,
		PropHorizontalReadoutSpeed,
		PropTemperatureSetPoint,
		PropCoolerOn,
		PropTriggerMode
	};

	enum AndorTriggerMode {
		TrigInternal = 0,
		TrigExternal = 1,
		TrigExternalStart = 6,
		TrigExternalExposure = 7
	};

	AndorCamera();
	virtual ~AndorCamera();

	std::string getIdentifierStr() const override;

	double getFrameRate() const override;

private:
	std::vector<CameraProperty> _derivedGetCameraProperties() override;
	void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

	bool _derivedIsConfiguredForHardwareTriggering() override;
	std::pair<int, int> _getSizeOfRawImages() const override;

	std::pair<int, int> _getSensorSize() const;
	AndorCapabilities _getCapabilities() const;
	bool _hasFrameTransferMode() const;
	bool _hasEMGain() const;
	CameraProperty _getSetFrameTransferMode(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetEMGain(GetOrSetProperty getOrSet, const double setPoint);
	CameraProperty _getSetHorizontalReadoutSpeeds(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetVerticalReadoutSpeeds(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetCoolerOn(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTemperatureSetPoint(GetOrSetProperty getOrSet, const double setPoint);
	CameraProperty _getSetTriggerMode(GetOrSetProperty getOrSet, const std::string& mode);
	std::vector<std::pair<std::string, int>> _triggerModes() const;

	void _setExposureTime(const double exposureTime);
	double _getExposureTime() const;
	void _setCropping(const std::pair<int, int>& crop);
	void _setBinning(const int binningFactor);
	void _setCroppingAndBinning(const std::pair<int, int>& crop, const int binningFactor);

	void _setDefaults();
	std::string _andorErrorCodeToMessage(int errorCode) const;

	bool _hasCustomAcquireSingleImage() const override { return true; }
	void _derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) override;

	void _derivedStartUnboundedAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	NewImageResult _waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) override;

	std::pair<int, int> _desiredCropSize;
	int _desiredBinningFactor;
	bool _frameTransferModeOn;
	bool _coolerOn;
	double _temperatureSetpoint;
	int _horizontalReadoutSpeedIndex;
	int _verticalReadoutSpeedIndex;
	int _triggerMode;
	int _numberOfImagesDelivered;
};

#endif
