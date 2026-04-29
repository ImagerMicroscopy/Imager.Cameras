#ifndef SRC_SCCONFIGURATIONFILE_H
#define SRC_SCCONFIGURATIONFILE_H

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "ImagerPluginCore/ConfigManager.h"

class ImageProcessingDescriptor;

std::vector<std::shared_ptr<ImageProcessingDescriptor>> GetProcessingOptionsForCamera(ConfigManager& configManager, const std::string &cameraName);


#endif //SRC_SCCONFIGURATIONFILE_H