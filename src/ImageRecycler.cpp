#include "ImageRecycler.h"

#include <algorithm>
#include <stdexcept>
#include <thread>
#include <mutex>


class ImageRecycler {
public:
    ImageRecycler() { ; }
    ~ImageRecycler();

    std::shared_ptr<std::uint16_t> newRecycledImage(std::pair<size_t, size_t> size);
    void freeAllImages();

private:
    void _returnImage(std::uint16_t* imagePtr);

    std::vector<std::pair<size_t, size_t>> _availableDimensions;
    std::vector<std::deque<std::shared_ptr<std::uint16_t>>> _availableImages;
    std::vector<std::deque<std::shared_ptr<std::uint16_t>>> _imagesInUse;
    std::mutex _mutex;
};

ImageRecycler gImageRecycler;

std::shared_ptr<std::uint16_t> NewRecycledImage(std::pair<size_t, size_t> size) {
    return gImageRecycler.newRecycledImage(size);
}

ImageRecycler::~ImageRecycler() {
    freeAllImages();
}

std::shared_ptr<std::uint16_t> ImageRecycler::newRecycledImage(std::pair<size_t, size_t> size) {
    std::lock_guard<std::mutex> lock(_mutex);

    // do we already have images of this size available?
    std::vector<std::pair<size_t, size_t>>::const_iterator it;
    if ((it = std::find(_availableDimensions.cbegin(), _availableDimensions.cend(), size)) == _availableDimensions.cend()) {
        // no images of this dimensions seen yet
        _availableDimensions.push_back(size);
        _availableImages.push_back(std::deque<std::shared_ptr<std::uint16_t>>());
        _imagesInUse.push_back(std::deque<std::shared_ptr<std::uint16_t>>());
        it = _availableDimensions.cend() - 1;
    }

    size_t sizeIdx = it - _availableDimensions.cbegin();
    if (_availableImages.at(sizeIdx).empty()) {
        _availableImages.at(sizeIdx).emplace_front(new std::uint16_t[size.first * size.second], [](std::uint16_t* p) {delete[] p; });
    }

    std::shared_ptr<std::uint16_t> theImage = _availableImages.at(sizeIdx).front();
    _availableImages.at(sizeIdx).pop_front();
    _imagesInUse.at(sizeIdx).push_back(theImage);

    std::shared_ptr<std::uint16_t> imageToReturn(theImage.get(), [=](std::uint16_t* p) {this->_returnImage(p); });
    return imageToReturn;
}

void ImageRecycler::freeAllImages() {
    std::lock_guard<std::mutex> lock(_mutex);
    for (const auto& v : _imagesInUse) {
        if (!v.empty()) {
            throw std::logic_error("recycler freeing all images but still in use");
        }
    }
    _availableDimensions.clear();
    _availableImages.clear();
    _imagesInUse.clear();
}

void ImageRecycler::_returnImage(std::uint16_t* ptr) {
    std::lock_guard<std::mutex> lock(_mutex);

    bool foundIt = false;
    size_t sizeIdx = 0;
    std::deque<std::shared_ptr<std::uint16_t>>::iterator it;
    for (sizeIdx = 0; sizeIdx < _imagesInUse.size(); sizeIdx += 1) {
        std::deque<std::shared_ptr<std::uint16_t>>& theQueue = _imagesInUse.at(sizeIdx);
        it = std::find_if(theQueue.begin(), theQueue.end(), [&](std::shared_ptr<std::uint16_t> p) {return (p.get() == ptr); });
        if (it != theQueue.end()) {
            foundIt = true;
            break;
        }
    }
    if (!foundIt) {
        throw std::logic_error("_returnImage() but unknown image");
    }

    _availableImages.at(sizeIdx).push_back(*it);
    _imagesInUse.at(sizeIdx).erase(it);
}
