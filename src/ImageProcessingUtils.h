#ifndef IMAGEPROCESSINGUTILS_H
#define IMAGEPROCESSINGUTILS_H

#include <cstddef>
#include <cstdint>

void RotateCW(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* rotatedImage);
void RotateCCW(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* rotatedImage);

void FlipHorizontal(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* flippedImage);
void FlipVertical(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* flippedImage);

void CropImage(const std::uint16_t* image, size_t nRows, size_t nCols, size_t outputNRows, size_t outputNCols, std::uint16_t* croppedImage);
void BinImage(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* binnedImage, const int binFactor);

enum ImageProcessingTypes {
    kRotateCW = 0,
    kRotateCCW = 1,
    kFlipHorizontal = 2,
    kFlipVertical = 3,
    kCrop = 4,
    kBin = 5
};

class ImageProcessingDescriptor {
public:
    virtual ImageProcessingTypes getType() const = 0;
};

class IPDRotateCW : public ImageProcessingDescriptor {
public:
    ImageProcessingTypes getType() const override { return kRotateCW; }
};

class IPDRotateCCW : public ImageProcessingDescriptor {
public:
    ImageProcessingTypes getType() const override { return kRotateCCW; }
};

class IPDFlipHorizontal : public ImageProcessingDescriptor {
public:
    ImageProcessingTypes getType() const override { return kFlipHorizontal; }
};

class IPDFlipVertical : public ImageProcessingDescriptor {
public:
    ImageProcessingTypes getType() const override { return kFlipVertical; }
};

class IPDBin : public ImageProcessingDescriptor {
public:
    IPDBin(int binFactorRHS) : binFactor(binFactorRHS) {}
    ImageProcessingTypes getType() const override { return kBin; }
    int binFactor;
};

class IPDCrop : public ImageProcessingDescriptor {
public:
    IPDCrop(size_t nRowsRHS, size_t nColsRHS) : nRows(nRowsRHS), nCols(nColsRHS) {}
    ImageProcessingTypes getType() const override { return kCrop; }
    size_t nRows, nCols;
};

#endif
