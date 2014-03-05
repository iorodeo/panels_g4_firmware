#include "i2c_message_buffer.h"
#include <avr/sfr_defs.h>

#ifdef WITH_SERIAL_DEBUG
#include <Streaming.h>
#endif

const uint16_t I2CMessageBuffer::bufferSizeInBits_ = uint16_t(I2C_BUFFER_SIZE)*8;

I2CMessageBuffer::I2CMessageBuffer()
{
    zero();
}


void I2CMessageBuffer::zero()
{
    for (uint8_t i=0; i<I2C_BUFFER_SIZE; i++)
    {
        data_[i] = 0;
    }
}


void I2CMessageBuffer::zeroFromBytePos(uint8_t bytePos)
{
    for (uint8_t i=bytePos; i<I2C_BUFFER_SIZE; i++)
    {
        data_[i] = 0;
    }
}


void I2CMessageBuffer::zeroFromBitPos(uint16_t bitPos)
{
    for (uint16_t i=bitPos; i<bufferSizeInBits_; i++)
    {
        setBit(i,0);
    }
}


void I2CMessageBuffer::setByte(uint8_t bytePos, uint8_t byteVal)
{
    if (bytePos < I2C_BUFFER_SIZE)
    {
        data_[bytePos] = byteVal;
    }
}


uint8_t I2CMessageBuffer::getByte(uint8_t bytePos)
{
    if (bytePos < I2C_BUFFER_SIZE)
    {
        return data_[bytePos];
    }
    else
    {
        return 0;
    }
}


void I2CMessageBuffer::setBit(uint16_t bitPos, uint8_t bitVal)
{
    if (bitPos < bufferSizeInBits_)
    {
        uint8_t buffPos = uint8_t(bitPos/8);
        uint8_t bytePos = uint8_t(bitPos%8);
        if (bitVal == 0)
        {
            data_[buffPos] &= ~_BV(bytePos);
        }
        else
        {
            data_[buffPos] |= _BV(bytePos);
        }
    }
}


uint8_t I2CMessageBuffer::getBit(uint16_t bitPos)
{
    if (bitPos < bufferSizeInBits_)
    {
        uint8_t buffPos = uint8_t(bitPos/8);
        uint8_t bytePos = uint8_t(bitPos%8);
        uint8_t bitVal = (data_[buffPos] & _BV(bytePos)) >> bytePos; 
        return bitVal;
    }
    else
    {
        return 0; // out of range - just return 0
    }
}


uint32_t I2CMessageBuffer::getUint32(uint8_t pos)
{
    if ((pos+3) < I2C_BUFFER_SIZE)
    {
        uint32_t value = 0;
        for (uint8_t i=0; i<4; i++)
        {
            value |= uint32_t(data_[i+pos]) << 8*i;
        }
        return value;
    }
    else
    {
        return 0;
    }
}


#ifdef WITH_SERIAL_DEBUG
void I2CMessageBuffer::printData()
{
    for (uint8_t i=0; i<I2C_BUFFER_SIZE; i++)
    {
        Serial << "i=" << i << ", val=" << getByte(i) << endl;
    }
}
#endif


