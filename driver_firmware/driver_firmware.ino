//#define WITH_SERIAL_DEBUG
#include <util/atomic.h>
#include <Wire.h>
#include <TimerOne.h>
#include "panels_g4.h"
#include "constants.h" 
#include "display_driver.h"
#include "i2c_handler.h"

#ifdef WITH_SERIAL_DEBUG
#include <Streaming.h>
#endif

void displayTest();

void setup()
{
#ifdef WITH_SERIAL_DEBUG
    Serial.begin(SERIAL_DEBUG_BAUDRATE);
#endif

    // Setup display driver
    displayDriver.initialize();
    displayDriver.setTimerCallback(timerCallback);
    displayDriver.startTimer();

    // Setup i2c message handler
    i2cHandler.initialize();
    i2cHandler.setReceiveEventCallback(i2cEventCallback);
}


void loop()
{
    //i2cHandler.processMsg();
    displayTest();
}


inline void timerCallback()
{
    displayDriver.updateDisplay();
}


inline void i2cEventCallback(int numBytes)
{
    i2cHandler.copyMsgToBuffer(numBytes);
}



// DEVEL
// ----------------------------------------------------------------------------
void displayTest()
{
    static uint8_t row=0;
    static uint8_t col=0;
    //I2CMessageBuffer msg;
    DisplayMatrix matrix;
    matrix.setAllZero();
    matrix.setNumGrayScaleValues(MAX_NUM_GRAYSCALE_VALUES);

    if (1)
    {
        matrix.setPixel(row,col,MAX_NUM_GRAYSCALE_VALUES-1);
        row++;
        if (row>=DISPLAY_MATRIX_SIZE)
        {
            row=0;
            col++;
            if (col >= DISPLAY_MATRIX_SIZE)
            {
                col = 0;
            }
        }
    }
    else
    {
        for (uint8_t i=0; i<DISPLAY_MATRIX_SIZE; i++)
        {
            for (uint8_t j=0; j<=i; j++)
            {
                matrix.setPixel(j,i,MAX_NUM_GRAYSCALE_VALUES-1);
            }
        }
    }

#ifdef WITH_SERIAL_DEBUG
    Serial << "initial matrix" << endl;
    matrix.printData();
    Serial << endl;
#endif

    I2CMessageBuffer msg = matrix.getMsgFromData();
    displayDriver.bufferSetDataFromMsg(msg);
    displayDriver.bufferCreateLookupTable();

#ifdef WITH_SERIAL_DEBUG
    Serial << "final matrix" << endl;
    displayDriver.bufferPrintMatrixData();
    Serial << endl;
#endif

    displayDriver.setSwapBufferFlag();
    delay(100);
}



