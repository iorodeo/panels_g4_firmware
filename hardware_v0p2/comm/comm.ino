#include <SPI.h>
#include "constants.h"
#include "utils.h"
#include "spi.h"
#include "mspim_spi.h"


void setup()
{
    initSlaveResetPins();
    resetAllSlaves();
    SPI_Initialize();    
    MSPIM_Initialize();  
    noInterrupts(); 
}


void loop()
{
    static Buffer buffer;
    //SPI_ReceiveMsg(buffer);

    // Testing
    // --------------------------------------------
    static uint8_t value = 0;
    static uint8_t updateCount = 0;
    //interrupts();
    delay(5);
    //noInterrupts();


    uint8_t msgSize = PWM_TYPE_16_MSG_SIZE;
    for (uint8_t i=0; i<NUM_SLAVE;i++)
    {
        buffer.insert(PWM_TYPE_16);

        for (uint8_t j=1; j<msgSize; j++)
        {
            //buffer.insert((value << 4) | value);
            buffer.insert(value);
        }
    }
    buffer.dataReady = true;
    updateCount++;
    if (updateCount >= 100)
    {
        updateCount = 0;
        value = (value + 1)%16;
    }
    // ---------------------------------------------

    MSPIM_SendDataToSlaves(buffer);
    buffer.clear();
}




