#include "display_driver.h"
#include <util/atomic.h>
#include <TimerOne.h>
#include "i2c_message_buffer.h"


// DisplayDriver public methods
// ----------------------------------------------------------------------------

DisplayDriver::DisplayDriver()
{
    activeBuffer_ = 0;
    writeBuffer_ = 1; 
    swapBufferFlag_ = false;
}


void DisplayDriver::initialize()
{
    setIOPortDDR();
    setDisplayOff();
    Timer1.initialize(DEFAULT_DISPLAY_TIMER_PERIOD);
}


void DisplayDriver::setTimerCallback(void(*timerCallback)())
{
    Timer1.attachInterrupt(timerCallback);
}

void DisplayDriver::startTimer()
{
    Timer1.start();
}


void DisplayDriver::stopTimer()
{
    Timer1.stop();
}


void DisplayDriver::setTimerPeriod(long period)
{
    Timer1.setPeriod(period);  // us
}


void DisplayDriver::setSwapBufferFlag() 
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        swapBufferFlag_ = true;
    }
}

void DisplayDriver::setDisplayOff()
{
    PinMask valueMask = {0x00, 0x00, 0x00};
    for (uint8_t i=0;i<DISPLAY_MATRIX_SIZE;i++)
    {
        valueMask.B |= DISPLAY_ROW_PIN_MASK[i].B;
        valueMask.C |= DISPLAY_ROW_PIN_MASK[i].C;
        valueMask.D |= DISPLAY_ROW_PIN_MASK[i].D;
    }
    PORTB = (~DISPLAY_ALL_PIN_MASK.B & PORTB) + (DISPLAY_ALL_PIN_MASK.B & valueMask.B);
    PORTC = (~DISPLAY_ALL_PIN_MASK.C & PORTC) + (DISPLAY_ALL_PIN_MASK.C & valueMask.C);
    PORTD = (~DISPLAY_ALL_PIN_MASK.D & PORTD) + (DISPLAY_ALL_PIN_MASK.D & valueMask.D);
}


void DisplayDriver::bufferSetPixel(uint8_t row, uint8_t col, uint8_t val)
{
    buffer_[writeBuffer_].setPixel(row,col,val);
}

uint8_t DisplayDriver::bufferGetPixel(uint8_t row, uint8_t col)
{
    return buffer_[writeBuffer_].getPixel(row,col);
}

void DisplayDriver::bufferSetRow(uint8_t row, uint8_t val)
{
    buffer_[writeBuffer_].setRow(row,val);
}

void DisplayDriver::bufferSetCol(uint8_t col, uint8_t val)
{
    buffer_[writeBuffer_].setCol(col,val);
}


void DisplayDriver::bufferSetAll(uint8_t val)
{
    buffer_[writeBuffer_].setAll(val);
}


void DisplayDriver::bufferSetAllZero()
{
    buffer_[writeBuffer_].setAllZero();
}


void DisplayDriver::bufferSetDisplayRotation(DisplayRotation value)
{
    buffer_[writeBuffer_].setDisplayRotation(value);
}


void DisplayDriver::bufferSetDisplayFlip(DisplayFlip value)
{
    buffer_[writeBuffer_].setDisplayFlip(value);
}


void DisplayDriver::bufferSetNumGrayScaleValues(uint8_t val)
{
    buffer_[writeBuffer_].setNumGrayScaleValues(val);
}


bool DisplayDriver::bufferSetDataFromMsg(I2CMessageBuffer &msg)
{
    return buffer_[writeBuffer_].setDataFromMsg(msg);
}


void DisplayDriver::bufferCreateLookupTable()
{
    buffer_[writeBuffer_].createLookupTable();
}


#ifdef WITH_SERIAL_DEBUG
void DisplayDriver::bufferPrintMatrixData()
{
    buffer_[writeBuffer_].printData();
}
#endif 


// DisplayDriver private methods
// ----------------------------------------------------------------------------

void DisplayDriver::setIOPortDDR()
{
    DDRB |= DISPLAY_ALL_PIN_MASK.B;
    DDRC |= DISPLAY_ALL_PIN_MASK.C;
    DDRD |= DISPLAY_ALL_PIN_MASK.D;
}


DisplayDriver displayDriver; 
