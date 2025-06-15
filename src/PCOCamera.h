#ifndef PCOCAMERA_H
#define PCOCAMERA_H

#include "BaseCameraClass.h"

#include <limits>
#include <string>
#include <vector>

#ifdef __linux__
    #define PCO_LINUX
#endif
#include "PCO/pco_linux_defs.h"
#include "PCO/pco_err.h"
#include "PCO/sc2_sdkstructures.h"
#include "PCO/sc2_sdkaddendum.h"
#include "PCO/sc2_camexport.h"
#include "PCO/sc2_defs.h"


#include "PCOAPIWrapper.h"

const int kPCOImagesInBuffer = 4;

class PCOCamera : public BaseCameraClass
{
public:
    enum GetOrSetProperty {
        GetProperty,
        SetProperty
    };

    enum PCOPropIDs {
        PropReadoutSpeed = CameraProperty::FirstAvailablePropertyID,
    };

    PCOCamera(HANDLE camHandle);
    virtual ~PCOCamera();

    std::string getIdentifierStr() override;

    double getFrameRate() override;

    static std::string pcoErrorAsString(const int errCode);

private:
    std::vector<CameraProperty> _derivedGetCameraProperties() override;
    void _derivedSetCameraProperties(const std::vector<CameraProperty> &properties) override;

    std::pair<int, int> _getSizeOfRawImages() override;

    std::pair<int, int> _getSensorSize() const { return _sensorSize; }
    // int _getBinningFactor() const override;
    CameraProperty _getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string &mode);

    void _setExposureTime(const double exposureTime);
    double _getExposureTime();

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> _derivedGetAdditionalImageProcessingDescriptors() override;

    bool _hasCustomAcquireSingleImage() const override { return false; }
    // void _derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) override;

    void _derivedStartUnboundedAsyncAcquisition() override {_derivedStartBoundedAsyncAcquisition(std::numeric_limits<std::uint64_t>::max());};
    bool _derivedHaveBoundedAsyncAcquisition() override {return true;}
    void _derivedStartBoundedAsyncAcquisition(std::uint64_t nImagesToAcquire) override;
    void _derivedAbortAsyncAcquisition() override;
    NewImageResult _waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) override;
    
    void _fetchCameraInfo();
    void _initDefaults();

    void _throwIfPCOError(int pcoErr) {if (pcoErr) throw std::runtime_error(pcoErrorAsString(pcoErr));}
    double _pcoTimeToSeconds(DWORD time, DWORD timeBase) const;

    HANDLE _camHandle;
    PCOAPIWrapper _pcoAPIWrapper;
    PCO_Description _camDescription;
    std::string _camName;
    std::pair<int, int> _sensorSize;
    std::pair<int, int> _desiredCropSize;
    int _desiredBinningFactor;
    std::map<std::string, int> _readoutSpeeds;
    std::vector<std::uint16_t> _frameBuffer;
    std::vector<HANDLE> _waitObjects;
    std::vector<DWORD> _bufferStatuses;
    size_t _nextBufferToReadIndex;
    int _numberOfImagesDelivered;
};

#endif
