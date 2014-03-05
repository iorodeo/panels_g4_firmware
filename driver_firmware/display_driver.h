#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H
#include "Arduino.h"
#include "constants.h"
#include "display_buffer.h"

class I2CMessageBuffer;

class DisplayDriver
{
    public:
        DisplayDriver();
        void initialize();
        void setTimerCallback(void (*timerCallback)());
        void startTimer();
        void stopTimer();
        void setTimerPeriod(long period);
        void setSwapBufferFlag();
        void setDisplayOff();
        void bufferSetPixel(uint8_t row, uint8_t col, uint8_t val);
        uint8_t bufferGetPixel(uint8_t row, uint8_t col);
        void bufferSetRow(uint8_t row, uint8_t val);
        void bufferSetCol(uint8_t col, uint8_t val);
        void bufferSetAll(uint8_t val);
        void bufferSetAllZero();
        void bufferSetDisplayRotation(DisplayRotation value);
        void bufferSetDisplayFlip(DisplayFlip value);
        void bufferSetNumGrayScaleValues(uint8_t val);
        void bufferCreateLookupTable();
        bool bufferSetDataFromMsg(I2CMessageBuffer &msg);
        inline void updateDisplay();

#ifdef WITH_SERIAL_DEBUG
        void bufferPrintMatrixData();
#endif

    private:
        DisplayBuffer buffer_[DISPLAY_NUM_BUFFER];
        volatile uint8_t activeBuffer_;
        volatile uint8_t writeBuffer_;
        volatile bool swapBufferFlag_;
        void setIOPortDDR();
};

inline void DisplayDriver::updateDisplay()
{
    // To be called in display update timer callback function
    static uint8_t pwm = 0;
    static uint8_t ind = 0;

    pwm += 1;
    if (pwm >= buffer_[activeBuffer_].numGrayScaleValues_)
    {
        pwm = 0;
        ind += 1; 
        if (ind >= DISPLAY_MATRIX_SIZE)
        {
            ind = 0;
            if (swapBufferFlag_)
            {
                activeBuffer_ = (activeBuffer_+1)%DISPLAY_NUM_BUFFER;
                writeBuffer_  = (activeBuffer_+1)%DISPLAY_NUM_BUFFER; 
                swapBufferFlag_ = false;
            }
        }
        PORTB = (DISPLAY_NOT_PIN_MASK.B & PORTB) + DISPLAY_ALL_COL_PIN_MASK.B;   
        PORTC = (DISPLAY_NOT_PIN_MASK.C & PORTC) + DISPLAY_ALL_COL_PIN_MASK.C;  
        PORTD = (DISPLAY_NOT_PIN_MASK.D & PORTD) + DISPLAY_ALL_COL_PIN_MASK.D;  
    }
    PORTB = (DISPLAY_NOT_PIN_MASK.B & PORTB) + buffer_[activeBuffer_].lookupTable_[ind][pwm].B;
    PORTC = (DISPLAY_NOT_PIN_MASK.C & PORTC) + buffer_[activeBuffer_].lookupTable_[ind][pwm].C;
    PORTD = (DISPLAY_NOT_PIN_MASK.D & PORTD) + buffer_[activeBuffer_].lookupTable_[ind][pwm].D;
}

extern DisplayDriver displayDriver; 

#endif
