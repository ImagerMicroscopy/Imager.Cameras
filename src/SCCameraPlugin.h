#ifndef SCCAMERAPLUGIN_H
#define SCCAMERAPLUGIN_H

#include <filesystem>

void InitPlugin(const std::filesystem::path& configDirPath);
void ShutdownPlugin();

#endif
