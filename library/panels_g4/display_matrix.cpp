#include "display_matrix.h"
#include <avr/sfr_defs.h>
#include "i2c_message_buffer.h"

#ifdef WITH_SERIAL_DEBUG
#include <Streaming.h>
#endif

// DisplayMatrix public methods
// ----------------------------------------------------------------------------

DisplayMatrix::DisplayMatrix()
{
    setAllZero();
    numGrayScaleValues_ = DEFAULT_NUM_GRAYSCALE_VALUES;
    rotation_ = DEFAULT_DISPLAY_ROTATION;
    flip_ = DEFAULT_DISPLAY_FLIP;
}


uint8_t DisplayMatrix::getPixel(uint8_t row, uint8_t col, bool flipAndRotate)
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


void DisplayMatrix::setPixel(uint8_t row, uint8_t col, uint8_t val, bool flipAndRotate)
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


void DisplayMatrix::setRow(uint8_t row, uint8_t val)
{
    for (uint8_t col=0; col<DISPLAY_MATRIX_SIZE; col++)
    {
        setPixel(row,col,val);
    }
}


void DisplayMatrix::setCol(uint8_t col, uint8_t val)
{
    for (uint8_t row=0; row<DISPLAY_MATRIX_SIZE; row++)
    {
        setPixel(row,col,val);
    }
}


void DisplayMatrix::setAll(uint8_t val)
{
    for (uint8_t row=0; row<DISPLAY_MATRIX_SIZE; row++)
    {
        setRow(row,val);
    }
}


void DisplayMatrix::setAllZero()
{
    setAll(0);
}


void DisplayMatrix::setDisplayRotation(DisplayRotation value)
{
    rotation_ = value;
}


void DisplayMatrix::setDisplayFlip(DisplayFlip value)
{
    flip_ = value;
}


void DisplayMatrix::setNumGrayScaleValues(uint8_t num)
{
    numGrayScaleValues_ = num;
}


uint8_t DisplayMatrix::getNumGrayScaleValues()
{
    return numGrayScaleValues_;
}


bool DisplayMatrix::setDataFromMsg(I2CMessageBuffer &msg)
{
    uint8_t cmdId = msg.getByte(0);
    if (cmdId != CMD_SET_DISPLAY_DATA)
    {
        return false; // cmd id incorrect
    }
    uint8_t numBytesInMsg = msg.getByte(1);
    if (numBytesInMsg <= 1)
    {
        return false; // too few bytes
    }
    uint8_t numGrayScaleValues = msg.getByte(2);
    if (numGrayScaleValues < MIN_NUM_GRAYSCALE_VALUES)  
    {
        return false; // too few grayscale values
    }
    if (numGrayScaleValues > MAX_NUM_GRAYSCALE_VALUES)
    {
        return false; // too many grayscale values
    }
    uint8_t numBytesForMatrix = getNumBytesForGrayScale(numGrayScaleValues);
    if (numBytesForMatrix != (numBytesInMsg - 1))
    {
        return false; // number of bytes in message incorrect
    }

    // Extract data from message
    setNumGrayScaleValues(numGrayScaleValues);
    uint8_t bitsPerElem = getNumBitsPerElem(numGrayScaleValues);
    uint8_t dataStartByte = 3;
    uint16_t bitCnt = dataStartByte*8; // matrix data starts wtih 3rd byte  

    for (uint8_t row=0; row<DISPLAY_MATRIX_SIZE; row++)
    {
        for (uint8_t col=0; col<DISPLAY_MATRIX_SIZE; col++)
        {
            uint8_t value = 0;
            for (uint8_t i=0; i<bitsPerElem; i++)
            {
                uint8_t bitVal = msg.getBit(bitCnt);
                value |= (bitVal << i);
                bitCnt++;
            }
            setPixel(row,col,value);
        }
    }
    return true;
}


I2CMessageBuffer DisplayMatrix::getMsgFromData()
{
    I2CMessageBuffer msg;
    uint8_t bitsPerElem = getNumBitsPerElem(numGrayScaleValues_);
    uint8_t numDataBytesInMsg = getNumBytesForGrayScale(numGrayScaleValues_)+1;
    uint8_t dataStartByte = 3;
    uint16_t bitCnt = dataStartByte*8; 

    msg.setByte(0,CMD_SET_DISPLAY_DATA);
    msg.setByte(1,numDataBytesInMsg);
    msg.setByte(2,numGrayScaleValues_);
    msg.zeroFromBytePos(dataStartByte);  

    for (uint8_t row=0; row < DISPLAY_MATRIX_SIZE; row++)
    {
        for (uint8_t col=0; col < DISPLAY_MATRIX_SIZE; col++)
        {
            for (uint8_t i=0; i<bitsPerElem; i++)
            {
                uint8_t bitVal = (getPixel(row,col) & _BV(i)) >> i;
                msg.setBit(bitCnt,bitVal);
                bitCnt++;
            }
        }
    }
    msg.setLength(numDataBytesInMsg+2); 
    return msg;
}


#ifdef WITH_SERIAL_DEBUG
void DisplayMatrix::printData()
{
    for (uint8_t row=0; row<DISPLAY_MATRIX_SIZE; row++)
    {
        for (uint8_t col=0; col<DISPLAY_MATRIX_SIZE; col++)
        {
            uint8_t pixelValue = getPixel(row,col);
            Serial << _DEC(pixelValue) << " ";
        }
        Serial << endl;
    }
}
#endif


// DisplayMatrix protected methods
// ----------------------------------------------------------------------------
void DisplayMatrix::getFlippedRowAndCol(uint8_t &row, uint8_t &col)
{
    if ((flip_ == DISPLAY_FLIP_ROW)  || (flip_ == DISPLAY_FLIP_ROW_AND_COL))
    {
        row = (DISPLAY_MATRIX_SIZE-1) - row;
    }
    if ((flip_ == DISPLAY_FLIP_COL) || (flip_ == DISPLAY_FLIP_ROW_AND_COL))
    {
        col = (DISPLAY_MATRIX_SIZE-1) - col;
    }
}


void DisplayMatrix::getRotatedRowAndCol(uint8_t &row, uint8_t &col)
{
    uint8_t swapVar;
    switch (rotation_)
    {
        case DISPLAY_ROTATION_90:
            swapVar = row;
            row = (DISPLAY_MATRIX_SIZE-1) - col;
            col = swapVar; 
            break;

        case DISPLAY_ROTATION_180:
            row = (DISPLAY_MATRIX_SIZE-1) - row;
            col = (DISPLAY_MATRIX_SIZE-1) - col;
            break;

        case DISPLAY_ROTATION_270:
            swapVar = row;
            row = col;
            col = (DISPLAY_MATRIX_SIZE-1) - swapVar;
            break;

        default:
            break;
    }
}

// Utility functions
// ----------------------------------------------------------------------------

uint8_t getNumBitsPerElem(uint8_t numGrayScaleValues)
{
    uint8_t bitsPerElem = 0;
    for (uint8_t i=7; i>-1; i--)
    {
        uint8_t bitTest = ((numGrayScaleValues-1) >> i) & 0b00000001;
        if (bitTest > 0)
        {
            bitsPerElem = i+1;
            break;
        }
    } 
    return bitsPerElem;
}


uint8_t getNumBitsPerRow(uint8_t numGrayScaleValues)
{
    uint8_t bitsPerElem = getNumBitsPerElem(numGrayScaleValues);
    uint8_t bitsPerRow = DISPLAY_MATRIX_SIZE*bitsPerElem;
    return bitsPerRow;
}


uint8_t getNumBytesPerRow(uint8_t numGrayScaleValues)
{
    uint8_t bitsPerRow = getNumBitsPerRow(numGrayScaleValues);
    uint8_t bytesPerRow = bitsPerRow/8;
    return bytesPerRow;
}


uint8_t getNumBytesForGrayScale(uint8_t numGrayScaleValues)
{
    uint8_t numBytesPerRow = getNumBytesPerRow(numGrayScaleValues);
    uint8_t numBytes = DISPLAY_MATRIX_SIZE*numBytesPerRow;
    return numBytes;
}


