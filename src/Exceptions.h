#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>

class AcquisitionTimeOutError : public std::runtime_error {
public:
    AcquisitionTimeOutError(const std::string& str) :
        std::runtime_error(str)
    {}
};

#endif
