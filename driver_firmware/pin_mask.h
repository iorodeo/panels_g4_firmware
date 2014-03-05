#ifndef PIN_MASK_H
#define PIN_MASK_H
#include <stdint.h>

struct PinMask
{
    uint8_t B;
    uint8_t C;
    uint8_t D;
}; 

PinMask getDisplayAllPinMask();
PinMask getDisplayAllColPinMask();
PinMask getDisplayAllRowPinMask();
PinMask getDisplayNotPinMask();
PinMask getDisplayNotColPinMask();
PinMask getDisplayNotRowPinMask();

#endif
