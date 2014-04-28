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
        bool useLookupTable();
        inline void updateDisplay();

        void setSwapIntFlag();
        void setNewDataReady();
        void checkForSwap();

#ifdef WITH_SERIAL_DEBUG
        void bufferPrintMatrixData();
#endif

    private:
        DisplayBuffer buffer_[DISPLAY_NUM_BUFFER];
        volatile uint8_t activeBuffer_;
        volatile uint8_t writeBuffer_;
        volatile bool swapBufferFlag_;
        volatile bool swapIntFlag_;
        volatile bool newDataReady_;
        bool useLookupTable_;
        void setIOPortDDR();
};

inline void DisplayDriver::updateDisplay()
{
    // To be called in display update timer callback function
    static uint8_t pwm = 0;
    static uint8_t row = 0;

    pwm += 1;
    if (pwm >= buffer_[activeBuffer_].numGrayScaleValues_)
    {
        pwm = 0;
        row += 1; 
        if (row >= DISPLAY_MATRIX_SIZE)
        {
            row = 0;
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

    if (useLookupTable_)
    {
        PORTB = (DISPLAY_NOT_PIN_MASK.B & PORTB) + buffer_[activeBuffer_].lookupTable_[row][pwm].B;
        PORTC = (DISPLAY_NOT_PIN_MASK.C & PORTC) + buffer_[activeBuffer_].lookupTable_[row][pwm].C;
        PORTD = (DISPLAY_NOT_PIN_MASK.D & PORTD) + buffer_[activeBuffer_].lookupTable_[row][pwm].D;
    }
    else
    {
        PinMask colValues = {0x00, 0x00, 0x00};
        for (uint8_t col=0; col<DISPLAY_MATRIX_SIZE; col++)
        {
            uint8_t val = buffer_[activeBuffer_].getPixel(row,col,false);
            if (pwm < val)
            {
                colValues.B |= DISPLAY_COL_PIN_MASK[col].B;
                colValues.C |= DISPLAY_COL_PIN_MASK[col].C;
                colValues.D |= DISPLAY_COL_PIN_MASK[col].D;
            }
        }

        PinMask outputVals;

        outputVals.B = (DISPLAY_ALL_COL_PIN_MASK.B & ~colValues.B) + 
            (DISPLAY_ALL_ROW_PIN_MASK.B & DISPLAY_ROW_PIN_MASK[row].B);

        outputVals.C = (DISPLAY_ALL_COL_PIN_MASK.C & ~colValues.C) + 
            (DISPLAY_ALL_ROW_PIN_MASK.C & DISPLAY_ROW_PIN_MASK[row].C);

        outputVals.D = (DISPLAY_ALL_COL_PIN_MASK.D & ~colValues.D) + 
            ( DISPLAY_ALL_ROW_PIN_MASK.D & DISPLAY_ROW_PIN_MASK[row].D);

        PORTB = (DISPLAY_NOT_PIN_MASK.B & PORTB) + outputVals.B;
        PORTC = (DISPLAY_NOT_PIN_MASK.C & PORTC) + outputVals.C;
        PORTD = (DISPLAY_NOT_PIN_MASK.D & PORTD) + outputVals.D;
    }
}

extern DisplayDriver displayDriver; 

#endif
