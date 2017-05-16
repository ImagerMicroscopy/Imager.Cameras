#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <functional>

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

#endif
