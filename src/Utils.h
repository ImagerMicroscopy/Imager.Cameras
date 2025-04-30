#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <string>

template <typename T>
T clamp(const T& a, const T& min, const T& max) {
	return std::min(max, std::max(a, min));
}

class CleanupRunner {
public:
    CleanupRunner(std::function<void()> func) : _func(func) {}
    ~CleanupRunner() { _func(); }

private:
    std::function<void()> _func;
};

class AtomicString {
public:
    AtomicString() {;}

    void set(const std::string& val);
    std::string get();
    void clear();
    bool empty();
private:
    std::string _theString;
    std::mutex _mutex;
};

std::string wcharStringToUtf8(const std::wstring& str);
std::wstring utf8StringToWChar(const std::string& str);

#endif
