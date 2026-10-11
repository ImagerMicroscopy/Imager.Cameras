#ifndef SRC_SCCONFIGURATIONFILE_H
#define SRC_SCCONFIGURATIONFILE_H

#include <memory>
#include <string>
#include <vector>

class ImageProcessingDescriptor;

// Parses an orientation options string (semicolon-separated tokens, e.g.
// "RotateCW;FlipVertical") into image processing descriptors.
// Throws std::runtime_error on an invalid token.
std::vector<std::shared_ptr<ImageProcessingDescriptor>> ParseOrientationOptions(const std::string& orientationStr);

#endif //SRC_SCCONFIGURATIONFILE_H

