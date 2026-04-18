#ifndef IDSPEAKCAMERA_H
#define IDSPEAKCAMERA_H

#include <optional>

#include "IDSPeakAPIWrapper.h"

#include "ImagerPluginCore/BaseCameraClass.h"

class IDSPeakCamera : public BaseCameraClass {
public:
    enum GetOrSetProperty {
        GetProperty,
        SetProperty
    };

    enum IDSPeakPropIDs {
        PropPixelClock = CameraProperty::FirstAvailablePropertyID,
        PropGain
    };

    IDSPeakCamera(const peak_camera_descriptor& camDescriptor);
    virtual ~IDSPeakCamera();

    std::string getIdentifierStr() override;

    double getFrameRate() override;

private:
    std::vector<CameraProperty> _derivedGetCameraProperties() override;
    void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

    CameraProperty _getSetPixelClock(std::optional<CameraProperty> maybeValueToSet = std::optional<CameraProperty>());
    CameraProperty _getSetGain(std::optional<CameraProperty> maybeValueToSet = std::optional<CameraProperty>());

    std::pair<int, int> _getSizeOfRawImages();

    void _setExposureTime(const double exposureTime);
    double _getExposureTime();
    void _setImageCrop(const std::pair<int, int>& crop);
    std::pair<int, int> _getImageCrop();

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> _derivedGetAdditionalImageProcessingDescriptors() override;

    void _derivedStartBoundedAsyncAcquisition(std::uint64_t nImagesToAcquire) override;
    void _derivedAbortAsyncAcquisition() override;
    std::optional<AcquiredImage> _waitForNewImageWithTimeout(int timeoutMillis) override;

    IDSPeakAPIWrapper _peakAPIWrapper;
    peak_camera_descriptor _camDescriptor;
    peak_camera_handle _camHandle;
    int _nextExpectedImageInSequenceIdx;

    std::pair<int, int> _cropSize;
};

#endif