#ifndef ANDORSDK3CAMERA_H
#define ANDORSDK3CAMERA_H

#include <optional>

#include "Andor/atcore.h"

#include "BaseCameraClass.h"

class AndorSDK3Camera : public BaseCameraClass {
public:

	enum AndorSDK3PropIDs {
		PropPixelClock = CameraProperty::FirstAvailablePropertyID,
	};

	enum TriggerMode {
		kTriggerInternal,
		kTriggerSoftware
	};

	class FloatValue {
	public:
		FloatValue(double curr, double min, double max) :
			currentValue(curr), minValue(min), maxValue(max) {}
		double currentValue;
		double minValue;
		double maxValue;
	};

	class IntValue {
	public:
		IntValue(int curr, int min, int max) :
			currentValue(curr), minValue(min), maxValue(max) {}
		int currentValue;
		int minValue;
		int maxValue;
	};

	const size_t kNBufferImages = 10;

	AndorSDK3Camera(const AT_H camHandle);
	virtual ~AndorSDK3Camera();

	std::string getIdentifierStr() const override;

	double getFrameRate() const override;

private:
    void _setDefaults();
    
	std::vector<CameraProperty> _derivedGetCameraProperties() override;
	void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

	CameraProperty _getSetPixelClock(std::optional<CameraProperty> maybeValueToSet = std::optional<CameraProperty>());

	std::pair<int, int> _getSizeOfRawImages() const override;

	void _setExposureTime(const double exposureTime);
	double _getExposureTime() const;

	std::string _getSetPixelClock_SDK(std::optional<std::string> maybeClock = std::optional<std::string>());
	std::vector<std::string> _getPossiblePixelClocks() const;
	TriggerMode _getSetTriggerMode_SDK(std::optional<TriggerMode> maybeMode = std::optional<TriggerMode>());
	void _sendSoftwareTrigger();

	std::vector<std::shared_ptr<ImageProcessingDescriptor>> _derivedGetAdditionalImageProcessingDescriptors() override;

	bool _hasCustomAcquireSingleImage() const override { return true; }
	void _derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) override;
	void _stopSoftwareTriggeredAcquisitionIfRunning();

	void _derivedStartUnboundedAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	NewImageResult _waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) override;

	void _startUnboundedAsyncAcquisitionWithTriggerMode(TriggerMode triggerMode);

	void _setParameterStringValue(const std::string& featureStr, const std::string& valueStr);
	std::string _getSelectedParameterStringValue(const std::string& featureStr) const;
	std::vector<std::string> _enumerateParameterStringValues(const std::string& featureStr) const;
	void _setParameterFloatValue(const std::string& featureStr, double value);
	FloatValue _getParameterFloatValue(const std::string& featureStr) const;
	void _setParameterIntValue(const std::string& featureStr, int value);
	IntValue _getParameterIntValue(const std::string& featureStr) const;
	void _setParameterBoolValue(const std::string& featureStr, bool value);
	bool _getParameterBoolValue(const std::string& featureStr) const;
	void _sendCommand(const std::string& command);

	AT_H _camHandle;
	std::vector<std::uint64_t> _bufferMemory;	// uint64_t to force 8 bit alignment required by the SDK
	std::vector<std::uint16_t*> _imageBufferPtrs;
	size_t _singleImageSizeInBytes;
	int _nextExpectedImageInSequenceIdx;
	bool _softwareTriggeredAcquisitionRunning;

	std::pair<int, int> _cropSize;
};

#endif
