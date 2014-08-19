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
    interrupts();
    delay(200);
    noInterrupts();

    for (uint8_t i=0; i<BUFFER_SIZE; i++)
    {
        buffer.data[i] = 0;
        buffer.dataLen++;
    }

    uint8_t msgSize = SLAVE_TYPE_2_MSG_SIZE;
    for (uint8_t i=0; i<NUM_SLAVE;i++)
    {
        buffer.insert(msgSize);
        for (uint8_t j=0; j<msgSize; j++)
        {
            buffer.insert(j);
        }
    }
    buffer.dataReady = true;
    // ---------------------------------------------

    MSPIM_SendDataToSlaves(buffer);
    buffer.clear();
}




