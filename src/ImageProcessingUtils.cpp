#include "ImageProcessingUtils.h"

#include <algorithm>
#include <cstring>
#include <limits>
#include <string>
#include <stdexcept>

#include "ippi.h"

void RotateCW(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* rotatedImage) {
    IppiSize roiSize = { (int)nRows, (int)nCols };
    ippiTranspose_16u_C1R(image, nRows * sizeof(std::uint16_t), rotatedImage, nCols * sizeof(std::uint16_t), roiSize);
    roiSize = { (int)nCols, (int)nRows };
    ippiMirror_16u_C1IR(rotatedImage, nCols * sizeof(std::uint16_t), roiSize, ippAxsVertical);
}

void RotateCCW(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* rotatedImage) {
    IppiSize roiSize = { (int)nRows, (int)nCols };
    ippiTranspose_16u_C1R(image, nRows * sizeof(std::uint16_t), rotatedImage, nCols * sizeof(std::uint16_t), roiSize);
    roiSize = { (int)nCols, (int)nRows };
    ippiMirror_16u_C1IR(rotatedImage, nCols * sizeof(std::uint16_t), roiSize, ippAxsHorizontal);
}

void FlipHorizontal(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* flippedImage) {
    IppiSize roiSize = { (int)nRows, (int)nCols };
    ippiMirror_16u_C1R(image, nRows * sizeof(std::uint16_t), flippedImage, nRows * sizeof(std::uint16_t), roiSize, ippAxsHorizontal);
}

void FlipVertical(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* flippedImage) {
    IppiSize roiSize = { (int)nRows, (int)nCols };
    ippiMirror_16u_C1R(image, nRows * sizeof(std::uint16_t), flippedImage, nRows * sizeof(std::uint16_t), roiSize, ippAxsVertical);
}

void CropImage(const std::uint16_t* image, size_t nRows, size_t nCols, size_t outputNRows, size_t outputNCols, std::uint16_t* croppedImage) {
    int rowOffset = (nRows - outputNRows) / 2;
    int colOffset = (nCols - outputNCols) / 2;
    const std::uint16_t* inputPtr = image + colOffset * nRows + rowOffset;
    IppiSize roiSize = { outputNRows, outputNCols };
    ippiCopy_16u_C1R(inputPtr, nRows * sizeof(std::uint16_t), croppedImage, outputNRows * sizeof(std::uint16_t), roiSize);
}

void BinImage(const std::uint16_t* image, size_t nRows, size_t nCols, std::uint16_t* binnedImage, const int binFactor) {
    if (binFactor > 32) {
        throw std::logic_error(std::string("unsupported binning factor"));
    }
    if (binFactor == 1) {
        memcpy(binnedImage, image, nRows * nCols * sizeof(std::uint16_t));
        return;
    }

    int nRowsOutput = nRows / binFactor;
    int nColsOutput = nCols / binFactor;
    int nPixelsOutput = nRowsOutput * nColsOutput;

    const std::uint16_t* inputPtr = image;
    std::uint16_t* outputPtr = binnedImage;

    for (int col = 0; col < nColsOutput; col += 1) {
        for (int row = 0; row < nRowsOutput; row += 1) {
            std::uint32_t accum = 0;
            for (int bCol = 0; bCol < binFactor; bCol += 1) {
                for (int bRow = 0; bRow < binFactor; bRow += 1) {
                    accum += *(inputPtr + bRow + bCol * nRows);
                }
            }
            *outputPtr = (accum > 65535) ? 65535 : *outputPtr;
            outputPtr += 1;
            inputPtr += binFactor;
        }
        inputPtr += nRows * (binFactor - 1);
    }
}
