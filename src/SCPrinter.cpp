
#include "SCPrinter.h"

#include <exception>
#include <stdexcept>

class SCPrinter {
public:
    SCPrinter() {}
    ~SCPrinter() {}

    SCPrinter(const SCPrinter &) = delete;
    SCPrinter &operator=(const SCPrinter &) = delete;

    void init(std::function<void(const std::string&)> printFunc) {
        if (_haveInit) {
            throw std::logic_error("SCPrinter already initialized");
        }
        _printFunc = printFunc;
        _haveInit = true;
    }

    void print(const std::string& str) {
        if (_haveInit) {
            std::lock_guard<std::mutex> lock(_printMutex);
            _printFunc(str);
        }
    }

private:
    std::function<void(const std::string&)> _printFunc = nullptr;
    std::mutex _printMutex;
    bool _haveInit = false;
};

SCPrinter gSCPrinter;

void InitPrinter(std::function<void(const std::string&)> printFunc) {
    gSCPrinter.init(printFunc);
}
void Print(const std::string& str) {
    gSCPrinter.print(str);
}
