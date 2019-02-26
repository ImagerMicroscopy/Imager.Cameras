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
	virtual ~AndorCamera();

	std::string getIdentifierStr() const override;

	std::vector<CameraProperty> getCameraProperties() override;
	void setCameraProperty(const CameraProperty& prop) override;

	std::pair<int, int> getActualImageSize() const override;
	double getFrameRate() const override;

private:
	std::pair<int, int> _getSensorSize() const override;
	AndorCapabilities _getCapabilities() const;
	bool _hasFrameTransferMode() const;
	bool _hasEMGain() const;
	CameraProperty _getSetFrameTransferMode(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetEMGain(GetOrSetProperty getOrSet, const double setPoint);
	CameraProperty _getSetHorizontalReadoutSpeeds(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetVerticalReadoutSpeeds(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetCoolerOn(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTemperatureSetPoint(GetOrSetProperty getOrSet, const double setPoint);

	void _setExposureTime(const double exposureTime) override;
	double _getExposureTime() const override;
	void _derivedSetImageCrop(const std::pair<int, int>& crop) override;
	void _derivedSetBinningFactor(const int binningFactor) override;
	int _getBinningFactor() const override;
	void _setCroppingAndBinning(const std::pair<int, int>& crop, const int binningFactor);

	bool _usesSoftwareCroppingAndBinning() const override { return false; };

	void _setDefaults();
	std::string _andorErrorCodeToMessage(int errorCode) const;

	bool _hasCustomAcquireSingleImage() const override { return true; }
	void _derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) override;

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
	std::pair<int, int> _desiredCropSize;
	int _desiredBinningFactor;
	int _numberOfImagesDelivered;
};

#endif
