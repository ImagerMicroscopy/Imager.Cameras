#include "SCConfigurationFile.h"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <sstream>
#include <stdexcept>

#include "ImageProcessingUtils.h"

std::vector<std::shared_ptr<ImageProcessingDescriptor>> GetProcessingOptionsForCamera(ConfigManager& configManager, const std::string& cameraName) {

    auto setting = configManager.getSettingOrDefault(cameraName / "orientation", "");
    std::string orientationStr = setting.first;

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> result;

    // Remove all whitespace from the orientation string
    std::string filtered;
    filtered.reserve(orientationStr.size());
    std::copy_if(orientationStr.begin(), orientationStr.end(), std::back_inserter(filtered), [](char c){
        return !std::isspace(static_cast<unsigned char>(c));
    });

    std::stringstream ss(filtered);
    std::string token;
    while (std::getline(ss, token, ';')) {
        if (token.empty()) continue;

        if (token == "RotateCW") {
            result.push_back(std::make_shared<IPDRotateCW>());
        } else if (token == "RotateCCW") {
            result.push_back(std::make_shared<IPDRotateCCW>());
        } else if (token == "FlipHorizontal") {
            result.push_back(std::make_shared<IPDFlipHorizontal>());
        } else if (token == "FlipVertical") {
            result.push_back(std::make_shared<IPDFlipVertical>());
        } else {
            throw std::runtime_error("Invalid orientation setting: " + token);
        }
    }

    return result;
}
