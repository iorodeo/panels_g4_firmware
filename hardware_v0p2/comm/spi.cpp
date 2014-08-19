#include "spi.h"
#include <Arduino.h>

void SPI_Initialize()
{
    pinMode(MISO,OUTPUT);
    pinMode(SS, INPUT);
    SPCR |= _BV(SPE);
}

void SPI_ReceiveMsg(Buffer &buffer)
{
    uint8_t msgSize;
    uint8_t pwmType;
    uint8_t delayValue;

    // Spin loop for receiving SPI messages
    // ------------------------------------------------------------------------
    while (!(SPSR & _BV(SPIF)));
    while (true)
    {
        // Read byte from spi data register
        buffer.data[buffer.dataLen] = SPDR;
        buffer.dataLen++;
        if (buffer.dataLen == 1)
        {
            msgSize = getBufferMsgSize(buffer);
        }
        if (buffer.dataLen == 4*msgSize)
        {
            buffer.dataReady = true;
            break;
        }
        while (!(SPSR & _BV(SPIF)));
    }
    while (digitalRead(SS) == 0); // Slow replace with direct port read

    // Read SPSR and SPDR a couple times - to clear out any possible mismatch 
    for (uint8_t i=0; i<5; i++)
    {
        uint8_t dummy0 = SPSR & _BV(SPIF);
        uint8_t dummy1 = SPDR;
    }
}

