#include "pin_mask.h"
#include "constants.h"


PinMask getDisplayAllPinMask()
{
    PinMask mask = {0x00, 0x00, 0x00};
    for (uint8_t i=0;i<DISPLAY_MATRIX_SIZE; i++) 
    {
        mask.B |= DISPLAY_COL_PIN_MASK[i].B; 
        mask.C |= DISPLAY_COL_PIN_MASK[i].C;
        mask.D |= DISPLAY_COL_PIN_MASK[i].D;
        mask.B |= DISPLAY_ROW_PIN_MASK[i].B;
        mask.C |= DISPLAY_ROW_PIN_MASK[i].C;
        mask.D |= DISPLAY_ROW_PIN_MASK[i].D;
    }
    return mask;
}


PinMask getDisplayAllColPinMask()
{
    PinMask mask = {0x00, 0x00, 0x00};
    for (uint8_t i=0;i<DISPLAY_MATRIX_SIZE; i++) 
    {
        mask.B |= DISPLAY_COL_PIN_MASK[i].B; 
        mask.C |= DISPLAY_COL_PIN_MASK[i].C;
        mask.D |= DISPLAY_COL_PIN_MASK[i].D;
    }
    return mask;
}


PinMask getDisplayAllRowPinMask()
{
    PinMask mask = {0x00, 0x00, 0x00};
    for (uint8_t i=0;i<DISPLAY_MATRIX_SIZE; i++) 
    {
        mask.B |= DISPLAY_ROW_PIN_MASK[i].B;
        mask.C |= DISPLAY_ROW_PIN_MASK[i].C;
        mask.D |= DISPLAY_ROW_PIN_MASK[i].D;
    }
    return mask;
}


PinMask getDisplayNotPinMask()
{
    PinMask allPinMask = getDisplayAllPinMask();
    PinMask notPinMask = {~allPinMask.B, ~allPinMask.C, ~allPinMask.D};
    return notPinMask;
}


PinMask getDisplayNotColPinMask()
{
    PinMask allColPinMask = getDisplayAllColPinMask();
    PinMask notColPinMask = {~allColPinMask.B, ~allColPinMask.C, ~allColPinMask.D};
    return notColPinMask;
}


PinMask getDisplayNotRowPinMask()
{
    PinMask allRowPinMask = getDisplayAllRowPinMask();
    PinMask notRowPinMask = {~allRowPinMask.B, ~allRowPinMask.C, ~allRowPinMask.D};
    return notRowPinMask;
}

