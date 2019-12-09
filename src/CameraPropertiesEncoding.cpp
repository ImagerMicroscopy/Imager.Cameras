#include "CameraPropertiesEncoding.h"

void CameraProperty::setNumeric(const double val) {
	if (_propertyType != CameraProperty::PropertyInvalid) {
		throw std::runtime_error("trying to reset encoded camera property");
	}
	_propertyType = CameraProperty::PropertyNumeric;
	_val = val;
}

void CameraProperty::setDiscrete(const std::string& currentOption, const std::vector<std::string>& availableOptions) {
	if (_propertyType != CameraProperty::PropertyInvalid) {
		throw std::runtime_error("trying to reset encoded camera property");
	}
	_propertyType = CameraProperty::PropertyDiscrete;
	_currentOption = currentOption;
	_availableOptions = availableOptions;
}

double CameraProperty::getValue() const {
	if (_propertyType != PropertyNumeric) {
		throw std::runtime_error("fetching numeric from discrete");
	}
	return _val;
}

std::string CameraProperty::getCurrentOption() const {
	if (_propertyType != PropertyDiscrete) {
		throw std::runtime_error("fetching discrete from numeric");
	}
	return _currentOption;
}

const std::vector<std::string>& CameraProperty::getAvailableOptions() const {
	if (_propertyType != PropertyDiscrete) {
		throw std::runtime_error("fetching discrete from numeric");
	}
	return _availableOptions;
}

nlohmann::json CameraProperty::encodeAsJSONObject() const {
	nlohmann::json encoded;
	encoded["propertycode"] = _propertyCode;
	encoded["descriptor"] = _descriptor;
	if (_propertyType == CameraProperty::PropertyNumeric) {
		encoded["kind"] = "numeric";
		encoded["value"] = _val;
	} else if (_propertyType == CameraProperty::PropertyDiscrete) {
		encoded["kind"] = "discrete";
		encoded["current"] = _currentOption;
		encoded["availableoptions"] = _availableOptions;
	} else {
		throw std::runtime_error("trying to encode unknown camera property type");
	}
	return encoded;
}

CameraProperty CameraProperty::decodeFromJSONObject(const nlohmann::json& encoded) {
	int propertyCode = encoded["propertycode"];
	std::string descriptor = encoded["descriptor"];
	CameraProperty cameraProperty(propertyCode, descriptor);
	std::string kind = encoded["kind"];
	if (kind == "numeric") {
		cameraProperty.setNumeric(encoded["value"]);
	} else if (kind == "discrete") {
		cameraProperty.setDiscrete(encoded["current"], encoded["availableoptions"]);
	} else {
		throw std::runtime_error("decoding camera property from incorrect object");
	}
	return cameraProperty;
}

std::vector<CameraProperty> GetStandardProperties(const double currentExposureTime, const std::pair<int, int>& currentCrop, const std::vector<std::pair<int,int>>& allowableCropping,
												  const int currentBinning, const std::vector<int>& allowableBinning) {
	std::vector<CameraProperty> properties;
	// exposure time
	{
		CameraProperty prop(CameraProperty::ReqPropExposureTime, "Exposure time");
		prop.setNumeric(currentExposureTime);
		properties.push_back(prop);
	}
	// cropping
	{
		int first, second;
		std::vector<std::string> strCropSizes;
		auto cropSizes = allowableCropping;
		for (const auto& size : cropSizes) {
			char buf[128];
			sprintf(buf, "%dx%d", size.first, size.second);
			strCropSizes.emplace_back(buf);
		}
		std::tie(first, second) = currentCrop;
		char actual[32];
		sprintf(actual, "%dx%d", first, second);
		CameraProperty prop(CameraProperty::ReqPropCropping, "Sensor cropping");
		prop.setDiscrete(actual, strCropSizes);
		properties.push_back(prop);
	}
	// binning
	{
		std::vector<std::string> binningStrs;
		std::string currentBinningStr;
		char buf[32];
		sprintf(buf, "%d", currentBinning);
		currentBinningStr = buf;
		for (const int b : allowableBinning) {
			sprintf(buf, "%d", b);
			binningStrs.push_back(buf);
		}
		CameraProperty prop(CameraProperty::ReqPropBinning, "Binning");
		prop.setDiscrete(currentBinningStr, binningStrs);
		properties.push_back(prop);
	}

	return properties;
}

std::tuple<std::optional<double>, std::optional<std::pair<int, int>>, std::optional<int>> DecodeAndRemoveStandardProperties(std::vector<CameraProperty>& properties) {
	std::optional<double> exposureTime;
	std::optional<std::pair<int, int>> cropping;
	std::optional<int> binning;

	for (int i = properties.size() - 1; i >= 0; i -= 1) {
		const CameraProperty& prop = properties.at(i);
		int propertyCode = prop.getPropertyCode();
		switch (propertyCode) {
			case CameraProperty::ReqPropExposureTime:
				exposureTime.emplace(prop.getValue());
				break;
			case CameraProperty::ReqPropCropping:
			{
				int first, second;
				if (sscanf(prop.getCurrentOption().c_str(), "%dx%d", &first, &second) != 2) {
					throw std::runtime_error("decoding cropping from invalid string");
				}
				cropping.emplace(first, second);
				break;
			}
			case CameraProperty::ReqPropBinning:
			{
				int theFactor = 0;
				if ((sscanf(prop.getCurrentOption().c_str(), "%d", &theFactor) != 1) || (theFactor <= 0)) {
					throw std::runtime_error("decoding binning from invalid string");
				}
				binning.emplace(theFactor);
				break;
			}
			default:
				continue;
				break;
		}
		properties.erase(properties.begin() + i);
		i -= 1;
	}

	return std::make_tuple(exposureTime, cropping, binning);
}

std::vector <std::pair<int, int>> StandardCroppingOptions(const std::pair<int, int>& uncroppedImageDimensions) {
	int cropDimensions[] = { 16,32,64,128,256,512,1024,1280,1536,2048,3072,4096 };
	std::vector<std::pair<int, int>> result;
	for (int s : cropDimensions) {
		if ((s < uncroppedImageDimensions.first) && (s < uncroppedImageDimensions.second)) {
			result.push_back(std::make_pair(s, s));
		}
	}
	result.push_back(uncroppedImageDimensions);
	return result;
}

std::vector<int> StandardBinningOptions() {
	return { 1, 2, 4 };
}
