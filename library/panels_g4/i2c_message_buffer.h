#ifndef I2C_MESSAGE_BUFFER_H
#define I2C_MESSAGE_BUFFER_H
#include <cstdint.h>
#include "common_constants.h"

class I2CHandler;

class I2CMessageBuffer
{
    friend class I2CHandler;

    public:
        I2CMessageBuffer();
        void setLength(uint8_t length);
        uint8_t getLength();
        void zero();
        void zeroFromBytePos(uint8_t bytePos);
        void zeroFromBitPos(uint16_t bitPos);
        void setBit(uint16_t bitPos, uint8_t bitVal);
        void setByte(uint8_t bytePos, uint8_t byteVal);
        uint8_t getBit(uint16_t bitPos);
        uint8_t getByte(uint8_t bytePos);
        uint32_t getUint32(uint8_t pos);
        uint8_t (&getDataArray())[I2C_BUFFER_SIZE];

#ifdef WITH_SERIAL_DEBUG
        void printData();
#endif

    private:
        uint8_t data_[I2C_BUFFER_SIZE];
        uint8_t length_;
        static const uint16_t bufferSizeInBits_;

};

#endif
