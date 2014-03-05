#include "constants.h"
#include <Arduino.h>

// Display constants
// ----------------------------------------------------------------------------
const uint32_t DEFAULT_DISPLAY_TIMER_PERIOD = 25;

// Pin masks for fast direct port IO
// ----------------------------------------------------------------------------
const PinMask DISPLAY_COL_PIN_MASK[DISPLAY_MATRIX_SIZE] = 
{
    { _BV(0),      0,      0 },
    { _BV(1),      0,      0 },
    { _BV(2),      0,      0 },
    { _BV(3),      0,      0 },
    { _BV(4),      0,      0 },
    { _BV(5),      0,      0 },
    {      0, _BV(0),      0 },
    {      0, _BV(1),      0 },
};

const PinMask DISPLAY_ROW_PIN_MASK[DISPLAY_MATRIX_SIZE] = 
{ 
#ifdef WITH_SERIAL_DEBUG
    {      0,      0,      0 },
    {      0,      0,      0 },
#else
    {      0,      0, _BV(0) },
    {      0,      0, _BV(1) },
#endif
    {      0,      0, _BV(2) },
    {      0,      0, _BV(3) },
    {      0,      0, _BV(4) },
    {      0,      0, _BV(5) },
    {      0,      0, _BV(6) },
    {      0,      0, _BV(7) },
}; 

const PinMask DISPLAY_ALL_PIN_MASK = getDisplayAllPinMask();
const PinMask DISPLAY_ALL_COL_PIN_MASK = getDisplayAllColPinMask();
const PinMask DISPLAY_ALL_ROW_PIN_MASK = getDisplayAllRowPinMask();
const PinMask DISPLAY_NOT_PIN_MASK = getDisplayNotPinMask();
const PinMask DISPLAY_NOT_COL_PIN_MASK = getDisplayNotColPinMask();
const PinMask DISPLAY_NOT_ROW_PIN_MASK = getDisplayNotRowPinMask();

// I2C communications
// ----------------------------------------------------------------------------
const uint8_t I2C_ADDRESS = 1;

