#ifndef DISPLAY_BUFFER_H
#define DISPLAY_BUFFER_H
#include "constants.h"
#include "display_matrix.h"

class DisplayDriver; 

class DisplayBuffer : public DisplayMatrix
{
    friend class DisplayDriver;

    public:
        DisplayBuffer();
        void createLookupTable(); 

    private:
        PinMask lookupTable_[DISPLAY_MATRIX_SIZE][MAX_NUM_GRAYSCALE_VALUES];
};

#endif
