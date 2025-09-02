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
    SCConfigurationFile(fs::path& configFileDirectory);
    ~SCConfigurationFile() {;}

    std::vector<ImageProcessingTypes> getProcessingOptionsForCamera(const std::string& cameraName);

private:
    static std::string _defaultConfigFileContents();
    void _parseConfigFile();
    ImageProcessingTypes _processingTypeForSetting(std::string argument);

    fs::path _configFilePath;
    std::map<std::string, std::vector<ImageProcessingTypes>>  _orientationOptions;
};


#endif //SRC_SCCONFIGURATIONFILE_H