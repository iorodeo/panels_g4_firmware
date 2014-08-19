#ifndef UTILS_H
#define UTILS_H
#include "constants.h"


class Buffer
{
    public:
        Buffer();
        void clear();
        uint8_t data[BUFFER_SIZE];
        uint8_t dataLen;
        bool dataReady;
        bool errorFlag;
};


void resetAllSlaves();
void initSlaveResetPins();

inline uint8_t getBufferMsgSize(Buffer &buffer)
{ 
    uint8_t pwmType = buffer.data[0] & PWM_TYPE_MASK; 
    if (pwmType == PWM_TYPE_16)
    {
        return SLAVE_TYPE_16_MSG_SIZE;
    }
    else
    {
        return SLAVE_TYPE_2_MSG_SIZE;
    }
}

#endif
