#ifndef CAMERAPROPERTIESENCODING_H
#define CAMERAPROPERTIESENCODING_H

#include <optional>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class CameraProperty {
public:
    enum PropertyType {
        PropertyInvalid,
        PropertyNumeric,
        PropertyDiscrete
    };

    enum StandardCameraPropertyIDs {
        ReqPropExposureTime,
        ReqPropCroppingDim1,
        ReqPropCroppingDim2,
        ReqPropBinning,
        FirstAvailablePropertyID
    };

    CameraProperty(const int propertyCode, const std::string& descriptor) :
        _propertyCode(propertyCode),
        _descriptor(descriptor),
        _propertyType(PropertyInvalid)
    {}
    ~CameraProperty() { ; }

    void setNumeric(const double val);
    void setDiscrete(const std::string& currentOption, const std::vector<std::string>& availableOptions);

    PropertyType getPropertyType() const { return _propertyType; }
    int getPropertyCode() const { return _propertyCode; }
    double getValue() const;
    std::string getCurrentOption() const;
    const std::vector<std::string>& getAvailableOptions() const;

    nlohmann::json encodeAsJSONObject() const;
    static CameraProperty decodeFromJSONObject(const nlohmann::json& encoded);

private:
    int _propertyCode;
    std::string _descriptor;
    PropertyType _propertyType;

    double _val;
    std::string _currentOption;
    std::vector<std::string> _availableOptions;
};

class DecodedStandardProperties {
    public:
        std::optional<double> exposureTime;
        std::optional<int> crop1;
        std::optional<int> crop2;
        std::optional<int> binningFactor;
};

std::vector<CameraProperty> GetStandardProperties(const double currentExposureTime, const std::pair<int, int>& currentCrop, const std::vector<int> &allowableCropping1,
                                                  const std::vector<int> &allowableCropping2, const int currentBinning, const std::vector<int>& allowableBinning);

DecodedStandardProperties DecodeAndRemoveStandardProperties(std::vector<CameraProperty>& properties);

std::vector<int> StandardCroppingOptions(int uncroppedDimension);
std::vector<int> StandardBinningOptions();

#endif
