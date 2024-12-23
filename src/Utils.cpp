#include "Utils.h"

std::string wcharStringToUtf8(const std::wstring& str) {
    size_t strlen = wcslen(str.c_str());

    int outLength = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), 0, 0, nullptr, nullptr);
    if (outLength < 0) {
        throw std::runtime_error("can't convert wide string to utf8");
    }
   
    std::string utf8Encoded(outLength, 0);
    outLength = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), strlen, utf8Encoded.data(), outLength, nullptr, nullptr);
    if (outLength < 0) {
        throw std::runtime_error("can't convert wide string to utf8");
    }
    
    return utf8Encoded;
}

std::wstring utf8StringToWChar(const std::string& str) {
    int convertResult = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
    if (convertResult < 0) {
        throw std::runtime_error("can't convert utf8 to wide string");
    }

    std::wstring wstring(convertResult, '0');
    convertResult = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), wstring.data(), wstring.size());
    if (convertResult <= 0) {
        throw std::runtime_error("can't convert utf8 to wide string");
    }

    return wstring;
}