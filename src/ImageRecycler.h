#ifndef IMAGERECYCLER_H
#define IMAGERECYCLER_H

#include <cstdint>
#include <queue>
#include <memory>
#include <vector>

std::shared_ptr<std::uint16_t> NewRecycledImage(std::pair<size_t, size_t> size);

#endif
