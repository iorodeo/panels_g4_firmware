#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H
#include <stdint.h>
#include <Wire.h>
#include "constants.h"
#include "i2c_message_buffer.h"


class I2CHandler 
{
    public:

        I2CHandler();
        void initialize();
        void setReceiveEventCallback(void(*receiveEventCallback)(int));
        inline void copyMsgToBuffer(int numBytes);
        void processMsg();

    private:
        I2CMessageBuffer msg_;
        uint8_t numItems_;
        bool haveNewMsg_;

};


// Inline methods
// ----------------------------------------------------------------------------

inline void I2CHandler::copyMsgToBuffer(int numBytes)
{
    // To be called in I2C event handler - otherwise should be in atomic block
    numItems_ = 0;
    while (Wire.available() > 0)
    {
        if (numItems_ < I2C_BUFFER_SIZE)
        {
            msg_.data_[numItems_] = Wire.read();
            numItems_++;
        }
    }
    if (numItems_ >= I2C_MIN_MESSAGE_SIZE)
    {
        haveNewMsg_ = true;
    }
}

extern I2CHandler i2cHandler;
#endif
