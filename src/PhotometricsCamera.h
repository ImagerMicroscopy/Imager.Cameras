#ifndef PHOTOMETRICSCAMERA_H
#define PHOTOMETRICSCAMERA_H

#include <tuple>
#include <tuple>

#include "PVCAM/master.h"
#include "PVCAM/pvcam.h"

#include "ImagerPluginCore/BaseCameraClass.h"
#include "PhotometricsAPIWrapper.h"

class PhotometricsCamera : public BaseCameraClass {

    enum GetOrSetProperty {
        GetProperty,
        SetProperty
    };

    enum PhotometricsPropIDs {
        PropReadoutPort = CameraProperty::FirstAvailablePropertyID,
        PropReadoutSpeed,
        PropGain,
        PropTriggerMode,
        PropPostProcessingFeature,
        PropFirstPostProcessingSettingID = CameraProperty::FirstAvailablePropertyID + 1000
    };

    class Gain {
    public:
        Gain(int index, int bitDepth, const std::string& description);

        int index() const { return _index; }
        const std::string& descriptor() const { return _description; }
        int bitDepth() const { return _bitDepth; }

    private:
        int _index;
        std::string _description;
        int _bitDepth;
    };
    
    class SpeedEntry {
    public:
        SpeedEntry(int indexRHS, int pixelTimeRHS, const std::vector<Gain>& gains) :
            _index(indexRHS),
            _pixelTime(pixelTimeRHS),
            _gains(gains)
        {
            _descriptor = _generateDescriptor();
        }

        int index() const { return _index; }
        const std::string& descriptor() const { return _descriptor; }
        const std::vector<Gain>& gains() const { return _gains; }

    private:
        std::string _generateDescriptor() const;

        int _index;
        int _pixelTime;
        std::string _descriptor;
        std::vector<Gain> _gains;
    };

    class ReadoutPort {
    public:
        ReadoutPort(const std::string& name, int index, const std::vector<SpeedEntry>& speedTable) :
            _name(name),
            _index(index),
            _speedTable(speedTable)
        {}

        const std::string& name() const { return _name; }
        int index() const { return _index; }
        const std::vector<SpeedEntry>& speedTable() const { return _speedTable; }

    private:
        std::string _name;
        int _index;
        std::vector<SpeedEntry> _speedTable;
    };

    class PostProcessingFeatureParameter {
    public:
        PostProcessingFeatureParameter(int pvcamID, int scCameraID, const std::string& descriptor) :
            _pvcamID(pvcamID),
            _scCameraID(scCameraID),
            _descriptor(descriptor)
        {}

        const int pvcamID() const { return _pvcamID; }
        const int scCameraID() const { return _scCameraID; }
        const std::string& descriptor() const { return _descriptor; }

    private:
        int _pvcamID;
        int _scCameraID;
        std::string _descriptor;
    };

    class PostProcessingFeature {
    public:
        PostProcessingFeature(int index, const std::string& description, const std::vector<PostProcessingFeatureParameter>& postProcessingSettings) :
            _index(index),
            _description(description),
            _postProcessingSettings(postProcessingSettings)
        {}

        int index() const { return _index; }
        const std::string& descriptor() const { return _description; }
        const std::vector<PostProcessingFeatureParameter> parameters() const { return _postProcessingSettings; }

    private:
        int _index;
        std::string _description;
        std::vector<PostProcessingFeatureParameter> _postProcessingSettings;
    };

public:
    PhotometricsCamera(const std::string& cameraName);
    ~PhotometricsCamera();

    std::string getIdentifierStr() override;

    double getFrameRate() override;

    static std::string getPVCAMErrorMessage();

private:
    void _setDefaults();

    std::vector<CameraProperty> _derivedGetCameraProperties() override;
    void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

    CameraProperty _getSetReadoutPort(GetOrSetProperty getOrSet, const std::string& port);
    CameraProperty _getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string& descriptor);
    CameraProperty _getSetGain(GetOrSetProperty getOrSet, const std::string& descriptor);
    CameraProperty _getSetTriggerMode(GetOrSetProperty getOrSet, const std::string& mode);
    CameraProperty _getSetPostProcessingFeature(GetOrSetProperty getOrSet, const std::string& mode);
    std::vector<CameraProperty> _getSetPostProcessingFeatureParameter(GetOrSetProperty getOrSet, const int scCameraParameterID, const double value);

    bool _derivedIsConfiguredForHardwareTriggering() override;

    std::pair<int, int> _getSizeOfRawImages() override;
    std::pair<int, int> _getSensorSize();

    double _getExposureTime();
    void _setExposureTime(const double exposureTime);
    void _updateCameraTimings();
    void _setImageCrop(const std::pair<int, int>& crop);
    std::pair<int, int> _getImageCrop() const;
    void _setBinningFactor(const int binningFactor);
    int _getBinningFactor() const;
    rgn_type _getRegionForCurrentBinningAndCropping();
    void _setTrigggerMode(const int triggerMode) { _triggerMode = triggerMode; }
    int _getTriggerMode() const { return _triggerMode; }
    std::vector<std::pair<std::string, int>> _getTriggerModes() const;

    void _derivedStartUnboundedAsyncAcquisition() override;
    void _derivedAbortAsyncAcquisition() override;
    NewImageResult _waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) override;

    static void _pvcamCallbackFunction(FRAME_INFO* infoPtr, void* contextPtr);
    static void _pvcamCameraRemovedCallbackFunction(FRAME_INFO* infoPtr, void* contextPtr);

    std::vector<ReadoutPort> _listReadoutPorts();
    std::tuple<ReadoutPort, SpeedEntry, Gain> _getCurrentReadoutSettings();
    std::vector<PostProcessingFeature> _listPostProcessingFeatures();
    const PostProcessingFeature& _getCurrentPostProcessingFeature();

    template <typename T> T _getCameraParameter(int paramID, int attribute) {
        T value;
        rs_bool result = _apiWrapper.pl_get_param(_pvcamHandle, paramID, attribute, &value);
        if (result != PV_OK) {
            throw std::runtime_error(getPVCAMErrorMessage());
        }
        return value;
    }

    void _fillCameraTextParameter(int paramID, char* buf) {
        rs_bool result = _apiWrapper.pl_get_param(_pvcamHandle, paramID, ATTR_CURRENT, buf);
        if (result != PV_OK) {
            throw std::runtime_error(getPVCAMErrorMessage());
        }
    }

    template <typename T> T _getCameraParameterCurrentValue(int paramID) {
        return _getCameraParameter<T>(paramID, ATTR_CURRENT);
    }

    bool _cameraSupportsParameter(int paramID) {
        rs_bool isAvailable = _getCameraParameter<rs_bool>(paramID, ATTR_AVAIL);
        return (isAvailable != PV_FAIL);
    }

    template <typename T> std::pair<T,T> _getCameraParameterLimits(int paramID) {
        T min = _getCameraParameter<T>(paramID, ATTR_MIN);
        T max = _getCameraParameter<T>(paramID, ATTR_MAX);
        return std::pair<T,T>(min, max);
    }

    std::uint32_t _getCameraParameterCount(int paramID) {
        return _getCameraParameter<std::uint32_t>(paramID, ATTR_COUNT);
    }
    std::vector<std::pair<std::int32_t, std::string>> _getCameraEnumParameters(int paramID);

    template <typename T> void _setCameraParameter(int paramID, T paramValue) {
        rs_bool result = _apiWrapper.pl_set_param(_pvcamHandle, paramID, &paramValue);
        if (result != PV_OK) {
            throw std::runtime_error(getPVCAMErrorMessage());
        }
    }

    PhotometricsAPIWrapper _apiWrapper;
    std::string _identifier;
    std::int16_t _pvcamHandle;
    std::vector<ReadoutPort> _readoutPorts;
    std::vector<PhotometricsCamera::PostProcessingFeature> _postProcessingFeatures;
    int _binningFactor;
    std::pair<int, int> _crop;
    int _triggerMode;
    std::vector<std::uint16_t> _asyncBuffer;
    moodycamel::BlockingConcurrentQueue<int> _pvcamCallbackQueue;
    bool _installedCallbackFunction;
    bool _haveCameraDisconnectionError;
};

#endif
