#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H
#include <stdint.h>
#include "constants.h"

class I2CMessageBuffer;


class EventDispatcher
{
    public:
        void newEvent(I2CMessageBuffer &msg);
        void swapDisplayBuffer();
        void enableDisplay();
        void disableDisplay();
        void setDisplayTimerPeriod(I2CMessageBuffer &msg);
        void setDisplayRotation(I2CMessageBuffer &msg);
        void setDisplayFlip(I2CMessageBuffer &msg);
        void setDisplayData(I2CMessageBuffer &msg);
        void setDisplayAllOn(I2CMessageBuffer &msg);
        void setDisplayAllOff();
        void getStatus();
};

extern EventDispatcher eventDispatcher;


#endif
