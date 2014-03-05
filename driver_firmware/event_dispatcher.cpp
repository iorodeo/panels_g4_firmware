#include "event_dispatcher.h"
#include <util/atomic.h>
#include "display_driver.h"
#include "i2c_message_buffer.h"

// Temporary
// ---------------------------
#ifdef WITH_SERIAL_DEBUG
#include <Arduino.h> 
#include <Streaming.h>
#endif
// ---------------------------

void EventDispatcher::newEvent(I2CMessageBuffer &msg)
{ 
    uint8_t cmdId = msg.getByte(0);

   switch (cmdId)
   {
       case CMD_SWAP_DISPLAY_BUFFER: 
           swapDisplayBuffer();
           break;

       case CMD_ENALBLE_DISPLAY: 
           enableDisplay();
           break;

       case CMD_DISABLE_DISPLAY: 
           disableDisplay();
           break;

       case CMD_SET_DISPLAY_TIMER_PERIOD: 
           setDisplayTimerPeriod(msg);
           break;

       case CMD_SET_DISPLAY_ROTATION: 
           setDisplayRotation(msg);
           break;

       case CMD_SET_DISPLAY_FLIP:
           setDisplayFlip(msg);
           break;

       case CMD_SET_DISPLAY_DATA:  
           setDisplayData(msg);
           break;

       case CMD_SET_DISPLAY_ALL_ON:
           setDisplayAllOn(msg);
           break;

       case CMD_SET_DISPLAY_ALL_OFF:
           setDisplayAllOff();
           break;

       case CMD_GET_STATUS:          
           getStatus();
           break;

       default:
           // unknown messsage - maybe send some kind of error?
           break;
   }
}


void EventDispatcher::swapDisplayBuffer()
{
#ifdef WITH_SERIAL_DEBUG
    Serial << "swap buffer" << endl; 
#endif
    displayDriver.setSwapBufferFlag();
}


void EventDispatcher::enableDisplay()
{
    displayDriver.setDisplayOff();
    displayDriver.startTimer();
}


void EventDispatcher::disableDisplay() 
{
    displayDriver.stopTimer();
    displayDriver.setDisplayOff();
}


void EventDispatcher::setDisplayTimerPeriod(I2CMessageBuffer &msg)
{
    uint8_t numBytes = msg.getByte(1);
    if (numBytes == 4)
    {
        uint32_t period = msg.getUint32(2);
        if (period <= DISPLAY_TIMER_MAX_PERIOD)
        {
            displayDriver.stopTimer();
            displayDriver.setTimerPeriod(period);
            displayDriver.startTimer();
        }
    }
} 


void EventDispatcher::setDisplayRotation(I2CMessageBuffer &msg)
{
    uint8_t numBytes = msg.getByte(1);
    if (numBytes == 1)
    {
        uint8_t value = msg.getByte(2);
        if (value < DISPLAY_ROTATION_MAXVAL)
        {
            displayDriver.bufferSetDisplayRotation(DisplayRotation(value));
            displayDriver.bufferCreateLookupTable();
        } 
    }
}


void EventDispatcher::setDisplayFlip(I2CMessageBuffer &msg)
{
    uint8_t numBytes = msg.getByte(1);
    if (numBytes == 1)
    {
        uint8_t value = msg.getByte(2);
        if (value < DISPLAY_FLIP_MAXVAL)
        {
            displayDriver.bufferSetDisplayFlip(DisplayFlip(value));
            displayDriver.bufferCreateLookupTable();
        }
    }
}


void EventDispatcher::setDisplayData(I2CMessageBuffer &msg)
{
    bool rtnStatus = displayDriver.bufferSetDataFromMsg(msg);
    if (rtnStatus == true)
    {
        displayDriver.bufferCreateLookupTable();
    }
} 


void EventDispatcher::setDisplayAllOn(I2CMessageBuffer &msg)
{

#ifdef WITH_SERIAL_DEBUG
    Serial << "set display all on" << endl; 
#endif

    uint8_t numBytes = msg.getByte(1);
    uint8_t numGrayScaleValues;
    uint8_t pixelValue;

    if (numBytes == 0)
    {
        numGrayScaleValues = MIN_NUM_GRAYSCALE_VALUES;
        pixelValue = numGrayScaleValues - 1;
    }
    else if (numBytes == 2)
    {
        uint8_t pixelValue = msg.getByte(1);
        uint8_t numGrayScaleValues = msg.getByte(2);
        numGrayScaleValues = max(numGrayScaleValues,MIN_NUM_GRAYSCALE_VALUES);
        pixelValue = min(pixelValue, numGrayScaleValues-1);
    }
    else
    {
        return;
    }
    displayDriver.bufferSetNumGrayScaleValues(numGrayScaleValues);
    displayDriver.bufferSetAll(pixelValue);
    displayDriver.bufferCreateLookupTable();
}


void EventDispatcher::setDisplayAllOff()
{
#ifdef WITH_SERIAL_DEBUG
    Serial << "set display all off" << endl; 
#endif

    displayDriver.bufferSetNumGrayScaleValues(MIN_NUM_GRAYSCALE_VALUES);
    displayDriver.bufferSetAllZero();
    displayDriver.bufferCreateLookupTable();
}


void EventDispatcher::getStatus()
{
    // ------------------------------------------------------------------------
    // TO DO
    // ------------------------------------------------------------------------ 
}


// Create EventDispatcher instance
// ---------------------------------------------------------------------------
EventDispatcher eventDispatcher;





