#ifndef DISPLAY_MATRIX_H
#define DISPLAY_MATRIX_H
#include <stdint.h>
#include "common_constants.h"

class I2CMessageBuffer;

class DisplayMatrix
{
    public:
        DisplayMatrix();
        inline void setPixel(uint8_t row, uint8_t col, uint8_t val, bool flipAndRotate=true);
        inline uint8_t getPixel(uint8_t row, uint8_t col, bool flipAndRotate=true);
        void setRow(uint8_t row, uint8_t val);
        void setCol(uint8_t col, uint8_t val);
        void setAll(uint8_t val);
        void setAllZero();
        void setNumGrayScaleValues(uint8_t num);
        void setDisplayRotation(DisplayRotation value);
        void setDisplayFlip(DisplayFlip value);
        bool setDataFromMsg(I2CMessageBuffer &msg);
        I2CMessageBuffer getMsgFromData();
        void getMsgFromData(I2CMessageBuffer &msg);
        uint8_t getNumGrayScaleValues();

#ifdef WITH_SERIAL_DEBUG
        void printData();
#endif

    protected:
        uint8_t matrixData_[DISPLAY_MATRIX_SIZE][DISPLAY_MATRIX_SIZE];
        uint8_t numGrayScaleValues_;
        DisplayRotation rotation_;
        DisplayFlip flip_;
        void getFlippedRowAndCol(uint8_t &row, uint8_t &col);
        void getRotatedRowAndCol(uint8_t &row, uint8_t &col);
};


inline uint8_t DisplayMatrix::getPixel(uint8_t row, uint8_t col, bool flipAndRotate)
{
    if ((row < DISPLAY_MATRIX_SIZE) && (col < DISPLAY_MATRIX_SIZE))
    {
        if (flipAndRotate)
        {
            getFlippedRowAndCol(row, col); 
            getRotatedRowAndCol(row, col);
        }
        return matrixData_[row][col];
    }
    else
    {
        return 0;
    }
}


inline void DisplayMatrix::setPixel(uint8_t row, uint8_t col, uint8_t val, bool flipAndRotate)
{
    if ((row < DISPLAY_MATRIX_SIZE) && (col < DISPLAY_MATRIX_SIZE))
    {
        if (flipAndRotate)
        {
            getFlippedRowAndCol(row, col); 
            getRotatedRowAndCol(row, col);
        }
        matrixData_[row][col] = val;
    }
}

// Utility functions
uint8_t getNumBitsPerElem(uint8_t numGrayScaleValues);
uint8_t getNumBitsPerRow(uint8_t numGrayScaleValues);
uint8_t getNumBytesPerRow(uint8_t numGrayScaleValues);
uint8_t getNumBytesForGrayScale(uint8_t numGrayScaleValues);

#endif
