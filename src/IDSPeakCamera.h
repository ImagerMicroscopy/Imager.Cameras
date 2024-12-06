#ifndef IDSPEAKCAMERA_H
#define IDSPEAKCAMERA_H

#include "ids_peak_comfort_c/ids_peak_comfort_c.h"

#include "BaseCameraClass.h"

class IDSPeakCamera : public BaseCameraClass {
public:
	enum GetOrSetProperty {
		GetProperty,
		SetProperty
	};

	enum IDSPeakPropIDs {
		PropEMMode = CameraProperty::FirstAvailablePropertyID,
		PropEMGain,
		PropReadoutSpeed,
		PropTemperatureSetPoint,
		PropCoolerOn,
		PropTriggerSource,
		PropTriggerMode,
		PropTriggerActive,
		PropTriggerPolarity
	};

	IDSPeakCamera(const peak_camera_descriptor& camDescriptor);
	virtual ~IDSPeakCamera();

	std::string getIdentifierStr() const override;

	double getFrameRate() const override;

private:
	std::vector<CameraProperty> _derivedGetCameraProperties() override;
	void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

	std::pair<int, int> _getSizeOfRawImages() const override;

	void _setExposureTime(const double exposureTime);
	double _getExposureTime() const;
    void _setImageCrop(const std::pair<int, int>& crop);
	std::pair<int, int> _getImageCrop();

	std::vector<std::shared_ptr<ImageProcessingDescriptor>> _derivedGetAdditionalImageProcessingDescriptors() override;

	bool _hasCustomAcquireSingleImage() const override { return false; }

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _waitForNewImageWithTimeout(int timeoutMillis) override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	peak_camera_descriptor _camDescriptor;
	peak_camera_handle _camHandle;
	peak_frame_handle _peakFrameH;
	int _nextExpectedImageInSequenceIdx;

	std::pair<int, int> _cropSize;;
};

#endif