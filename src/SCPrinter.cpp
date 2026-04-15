
#include "SCPrinter.h"

#include "ImagerPluginCore/PluginManager.h"


void Print(const std::string& str) {
    PluginManager::Manager().Print(str);
}
