#include "common_constants.h"
#include <Arduino.h>

// Display constants
// ----------------------------------------------------------------------------
const uint8_t DEFAULT_NUM_GRAYSCALE_VALUES = 2;
const uint8_t MIN_NUM_GRAYSCALE_VALUES = 2;
const uint32_t DISPLAY_TIMER_MAX_PERIOD = 8388480; // Max for Timer1
const DisplayRotation DEFAULT_DISPLAY_ROTATION = DISPLAY_ROTATION_0;
const DisplayFlip DEFAULT_DISPLAY_FLIP = DISPLAY_FLIP_NONE;

// I2C communications
// ----------------------------------------------------------------------------
const uint8_t I2C_MIN_MESSAGE_SIZE = 2;

// Serial Debugging
// ----------------------------------------------------------------------------
const long SERIAL_DEBUG_BAUDRATE = 115200;
