#include "SCConfigurationFile.h"

#include <stdexcept>

#include "toml.hpp"

SCConfigurationFile::SCConfigurationFile(fs::path &configFileDirectory) {
    _configFilePath = configFileDirectory / "SCCameraConfig.toml";

    if (!fs::exists(_configFilePath)) {
        std::ofstream file(_configFilePath, std::ios::trunc);
        file << _defaultConfigFileContents();
    }
}

std::vector<ImageProcessingTypes> SCConfigurationFile::getProcessingOptionsForCamera(const std::string &cameraName) {
    if (_orientationOptions.count(cameraName) > 0) {
        return _orientationOptions.at(cameraName);
    } else {
        return std::vector<ImageProcessingTypes>();
    }
}

std::string SCConfigurationFile::_defaultConfigFileContents() {
    std::string contents =
    R"(# Configuration file for the SCCamera plugin for Imager"
#
# Use this file to set any camera-specific properties. Imager will then set these
# properties when the program starts.
#

[Camera Orientation]
# Include orientation settings for as many cameras as you want. Specify the camera via the name with which
# it shows up in Imager. The names of the supported operations are "RotateCW", "RotateCCW", "FlipHorizontal", "FlipVertical".
# You can specify these names in an array. The operations will be applied on the images from left to right.
# The actual visual effect of the operations may not be what you expect, depending on how the images are displayed.
# The best way to figure this out for your system is to simply experiment.
#
# Example 1: Camera1 needs all four operations to be acceptable
# "Camera1" = ["RotateCW", "RotateCCW", "FlipHorizontal", "FlipVertical"]
# Example 2: Camera2 needs no operations to be acceptable
# "Camera2" = []
# Lines that start with '#' are treated as comments (ignored). Be sure to remove these when configuring your system!
)";
    return contents;
}

void SCConfigurationFile::_parseConfigFile() {
    try {
        // Load the TOML file
        const auto data = toml::parse(_configFilePath.string()); // Replace with the actual path to your TOML file

        // Extract the "Camera Orientation" table
        if (!data.contains("Camera Orientation")) {
            throw std::runtime_error("The config file is missing the \"Camera Orientation\" section. (Delete the file to generate a new one on startup");
        }

        const auto& cameraOrientationNode = data.at("Camera Orientation");
        const auto& cameraOrientation = cameraOrientationNode.as_table();
        // Iterate through each name and its associated transformations
        for (const auto& entry : *cameraOrientation) {
            const std::string cameraName = std::string(entry.first);
            std::vector<ImageProcessingTypes> orientationOptions;
            // Check if the node is an array and iterate over its elements
            if (auto transformations = entry.second.as_array()) {
                for (const auto& transformNode : *transformations) {
                    // Check if the element is a string before extracting
                    if (auto transformValue = transformNode.as_string()) {
                        // Here, you would use transformValue->get() or simply *transformValue
                        // to get the string, and pass it to _processingTypeForSetting
                        orientationOptions.push_back(_processingTypeForSetting(transformValue->get()));
                    }
                }
            }
            _orientationOptions[cameraName] = orientationOptions;
        }
    } catch (const toml::parse_error& err) {
        throw std::runtime_error(std::string("Failed to parse TOML file: ") + err.what());
    }
}

ImageProcessingTypes SCConfigurationFile::_processingTypeForSetting(const std::string argument) {
    std::vector<std::pair<std::string, ImageProcessingTypes>> processingOptions;
    processingOptions = {{"RotateCW", kRotateCW},
                         {"RotateCCW", kRotateCCW},
                         {"FlipHorizontal", kFlipHorizontal},
                         {"FlipVertical", kFlipVertical}};

    for (const auto& [name, option] : processingOptions) {
        if (argument == name) {
            return option;
        }
    }

    throw std::runtime_error("Invalid orientation setting: " + argument);
}
