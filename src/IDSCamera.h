#ifndef IDSCAMERA_H
#define IDSCAMERA_H

#include "BaseCameraClass.h"

#include <string>
#include <vector>

#include "IDS/uEye.h"

const int kIDSImagesInBuffer = 15;

class IDSCamera : public BaseCameraClass {
public:
    enum GetOrSetProperty {
        GetProperty,
        SetProperty
    };

    enum IDSPropIDs {
        PropGainBoost = CameraProperty::FirstAvailablePropertyID,
        PropHardwareGain,
        PropPixelClock,
        PropHotPixelCorrection,
        PropAdaptiveHotPixelCorrectionMode,
        PropAdaptiveHotPixelSensitivity,
    };

    IDSCamera(HIDS camHandle);
    ~IDSCamera();

    std::string getIdentifierStr() const override;

    double getFrameRate() const override;

private:
    std::vector<CameraProperty> _derivedGetCameraProperties() override;
    void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

    std::pair<int, int> _getSizeOfRawImages() const override;
    std::pair<int, int> _getSensorSize() const;
    double _getExposureTime() const;
    void _setExposureTime(const double exposureTime);

    CameraProperty _getSetGainBoost(GetOrSetProperty getOrSet, const std::string& mode);
    CameraProperty _getSetHardwareGain(GetOrSetProperty getOrSet, const double value);
    CameraProperty _getSetPixelClock(GetOrSetProperty getOrSet, const std::string& mode);
    CameraProperty _getSetHotPixelCorrection(GetOrSetProperty getOrSet, const std::string& mode);
    CameraProperty _getSetAdaptiveHotPixelCorrectionMode(GetOrSetProperty getOrSet, const std::string& mode);
    CameraProperty _getSetAdaptiveHotPixelCorrectionSensitivity(GetOrSetProperty getOrSet, const double value);

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> _derivedGetAdditionalImageProcessingDescriptors() override;

    bool _haveGainBoost() const;
    bool _haveHotPixelCorrection() const;

    void _derivedStartUnboundedAsyncAcquisition() override;
    void _derivedAbortAsyncAcquisition() override;
    NewImageResult _waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) override;

    void _setDefaults();

    HIDS _camHandle;
    std::pair<int, int> _desiredCropSize;
    int _desiredBinningFactor;
    std::vector<std::pair<int, std::string>> _supportedPixelClocks;
    std::vector<std::uint16_t> _frameBuffer;
    std::vector<char*> _frameBufferPointers;
    std::vector<int> _frameBufferIDs;
};

#endif
