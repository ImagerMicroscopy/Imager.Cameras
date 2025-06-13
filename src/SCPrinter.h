#ifndef SCPRINTER_H
#define SCPRINTER_H

#include <functional>
#include <mutex>
#include <string>

void InitPrinter(std::function<void(const std::string&)> printFunc);
void Print(const std::string& str);

#endif //SCPRINTER_H
