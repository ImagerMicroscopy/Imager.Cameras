#ifndef IMAGEPROCESSINGUTILS_H
#define IMAGEPROCESSINGUTILS_H

template <typename T>
void RotateCW(T* image, size_t nRows, size_t nCols, T* rotatedImage) {
    size_t offset = 0;
    for (size_t col = 0; col < nCols; ++col) {
        for (size_t row = 0; row < nRows; ++row) {
            size_t newRow = nCols - 1 - col;
            size_t newCol = row;
            size_t idx = RowColToLinear(nCols, newRow, newCol);
            rotatedImage[idx] = image[offset];
            offset += 1;
        }
    }
}

template <typename T>
void RotateCCW(T* image, size_t nRows, size_t nCols, T* rotatedImage) {
    size_t offset = 0;
    for (size_t col = 0; col < nCols; ++col) {
        for (size_t row = 0; row < nRows; ++row) {
            size_t newRow = col;
            size_t newCol = nRows - 1 - row;
            size_t idx = RowColToLinear(nCols, newRow, newCol);
            rotatedImage[idx] = image[offset];
            offset += 1;
        }
    }
}

template <typename T>
void FlipHorizontal(T* image, size_t nRows, size_t nCols, T* flippedImage) {
    size_t offset = 0;
    for (size_t col = 0; col < nCols; ++col) {
        for (size_t row = 0; row < nRows; ++row) {
            size_t newRow = row;
            size_t newCol = nCols - 1 - col;
            size_t idx = RowColToLinear(nRows, newRow, newCol);
            flippedImage[idx] = image[offset];
            offset += 1;
        }
    }
}

template <typename T>
void FlipVertical(T* image, size_t nRows, size_t nCols, T* flippedImage) {
    size_t offset = 0;
    for (size_t col = 0; col < nCols; ++col) {
        for (size_t row = 0; row < nRows; ++row) {
            size_t newRow = nRows - 1 - row;
            size_t newCol = col;
            size_t idx = RowColToLinear(nRows, newRow, newCol);
            flippedImage[idx] = image[offset];
            offset += 1;
        }
    }
}

inline size_t RowColToLinear(size_t nRows, size_t row, size_t col) {
    return (col * nRows + row);
}

#endif
