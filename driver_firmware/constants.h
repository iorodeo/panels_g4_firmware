#ifndef CONSTANTS_H 
#define CONSTANTS_H
#include "panels_g4.h"
#include <cstdint.h>
#include "pin_mask.h"

// Display constants
// ----------------------------------------------------------------------------
enum {DISPLAY_NUM_BUFFER = 2};
extern const uint32_t DEFAULT_DISPLAY_TIMER_PERIOD;

// Pin masks for fast direct port IO
// ----------------------------------------------------------------------------
extern const PinMask DISPLAY_COL_PIN_MASK[DISPLAY_MATRIX_SIZE]; 
extern const PinMask DISPLAY_ROW_PIN_MASK[DISPLAY_MATRIX_SIZE]; 
extern const PinMask DISPLAY_ALL_COL_PIN_MASK;
extern const PinMask DISPLAY_ALL_ROW_PIN_MASK;
extern const PinMask DISPLAY_ALL_PIN_MASK; 
extern const PinMask DISPLAY_NOT_PIN_MASK;
extern const PinMask DISPLAY_NOT_COL_PIN_MASK;
extern const PinMask DISPLAY_NOT_ROW_PIN_MASK;

// I2C communications
// ----------------------------------------------------------------------------
extern const uint8_t I2C_ADDRESS;

#endif
