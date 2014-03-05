#ifndef COMMON_CONSTANTS_H
#define COMMON_CONSTANTS_H
#include "Arduino.h"

// Command IDs
// ----------------------------------------------------------------------------
enum PanelCmdId
{
    CMD_SWAP_DISPLAY_BUFFER = 0,    // 0
    CMD_ENALBLE_DISPLAY,            // 1
    CMD_DISABLE_DISPLAY,            // 2
    CMD_SET_DISPLAY_TIMER_PERIOD,   // 3
    CMD_SET_DISPLAY_ROTATION,       // 4
    CMD_SET_DISPLAY_FLIP,           // 5
    CMD_SET_DISPLAY_DATA,           // 6
    CMD_SET_DISPLAY_ALL_ON,         // 7
    CMD_SET_DISPLAY_ALL_OFF,        // 8
    CMD_GET_STATUS,                 // 9
};

// Display constants
// ----------------------------------------------------------------------------
enum {DISPLAY_MATRIX_SIZE = 8};
#ifdef WITH_SERIAL_DEBUG
enum {MAX_NUM_GRAYSCALE_VALUES = 4};
#else
enum {MAX_NUM_GRAYSCALE_VALUES = 16};
#endif
extern const uint8_t DEFAULT_NUM_GRAYSCALE_VALUES;
extern const uint8_t MIN_NUM_GRAYSCALE_VALUES;
extern const uint32_t DISPLAY_TIMER_MAX_PERIOD;

enum DisplayFlip
{
    DISPLAY_FLIP_NONE=0,
    DISPLAY_FLIP_ROW,
    DISPLAY_FLIP_COL,
    DISPLAY_FLIP_ROW_AND_COL,
    DISPLAY_FLIP_MAXVAL, // for range checks
};
extern const DisplayFlip DEFAULT_DISPLAY_FLIP;

enum DisplayRotation
{
    DISPLAY_ROTATION_0=0,
    DISPLAY_ROTATION_90,
    DISPLAY_ROTATION_180,
    DISPLAY_ROTATION_270,
    DISPLAY_ROTATION_MAXVAL, // for range checks
};
extern const DisplayRotation DEFAULT_DISPLAY_ROTATION;

// I2C communications
// ----------------------------------------------------------------------------
enum {I2C_BUFFER_SIZE=42};
enum {I2C_NUM_SLAVES=4};
extern const uint8_t I2C_MIN_MESSAGE_SIZE;
extern const uint8_t I2C_SLAVE_ADDRESS[I2C_NUM_SLAVES];


// Serial Debugging
// ----------------------------------------------------------------------------
extern const long SERIAL_DEBUG_BAUDRATE;

#endif
