#include "i2c_handler.h"
#include <util/atomic.h>
#include "event_dispatcher.h"


// Temporary
// ---------------------------
#ifdef WITH_SERIAL_DEBUG
#include <Arduino.h> 
#include <Streaming.h>
#endif
// ---------------------------


I2CHandler::I2CHandler()
{
    numItems_ = 0;
    haveNewMsg_ = false;
}

void I2CHandler::initialize()
{
    Wire.begin(I2C_ADDRESS);
}


void I2CHandler::setReceiveEventCallback(void(*receiveEventCallback)(int))
{
    Wire.onReceive(receiveEventCallback);
}

void I2CHandler::processMsg()
{
    bool haveNewMsgCopy = false;
    I2CMessageBuffer msgCopy;

    // If there is a new message copy it from the receive buffer
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (haveNewMsg_)
        {
            // Don't need to copy the entire buffer
            for (uint8_t i=0; i<I2C_BUFFER_SIZE; i++)
            {
                msgCopy.data_[i] = msg_.data_[i];
            }
            haveNewMsgCopy = haveNewMsg_;
            haveNewMsg_ = false;
        }
    }

    // Process new message based on command id
    if (haveNewMsgCopy)
    {
#ifdef WITH_SERIAL_DEBUG
        Serial << "new message"  << endl;
#endif
        eventDispatcher.newEvent(msgCopy);

    }
}


// Create I2CHandler instance
// ----------------------------------------------------------------------------
I2CHandler i2cHandler;
