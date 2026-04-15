
#include "SCPrinter.h"

#include "PluginManager.h"


void Print(const std::string& str) {
    PluginManager::Manager().Print(str);
}
