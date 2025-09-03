#ifndef SRC_SCCONFIGURATIONFILE_H
#define SRC_SCCONFIGURATIONFILE_H

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "ImageProcessingUtils.h"

namespace fs = std::filesystem;

class SCConfigurationFile {
public:
    SCConfigurationFile(const fs::path& configFileDirectory);
    ~SCConfigurationFile() {;}

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> getProcessingOptionsForCamera(const std::string &cameraName);

private:
    static std::string _defaultConfigFileContents();
    void _parseConfigFile();

    std::shared_ptr<ImageProcessingDescriptor> _processingTypeForSetting(std::string argument);

    fs::path _configFilePath;
    std::map<std::string, std::vector<std::shared_ptr<ImageProcessingDescriptor>>>  _orientationOptions;
};


#endif //SRC_SCCONFIGURATIONFILE_H