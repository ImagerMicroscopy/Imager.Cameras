#ifndef UTILS_H
#define UTILS_H

#include <algorithm>

template <typename T>
T clamp(const T& a, const T& min, const T& max) {
	return std::min(max, std::max(a, min));
}

#endif
