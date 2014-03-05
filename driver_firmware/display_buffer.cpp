#include "display_buffer.h"
#include "Arduino.h"

DisplayBuffer::DisplayBuffer() : DisplayMatrix::DisplayMatrix()
{
    createLookupTable();
}

void DisplayBuffer::createLookupTable()
{
    for (uint8_t row=0; row<DISPLAY_MATRIX_SIZE; row++)
    {
        for (uint8_t pwmCount=0; pwmCount < numGrayScaleValues_; pwmCount++)
        {
            PinMask colValues = {0x00, 0x00, 0x00};
            for (uint8_t col=0; col<DISPLAY_MATRIX_SIZE; col++)
            {
                uint8_t val = getPixel(row,col,false);
                if (pwmCount < val)
                {
                    colValues.B |= DISPLAY_COL_PIN_MASK[col].B;
                    colValues.C |= DISPLAY_COL_PIN_MASK[col].C;
                    colValues.D |= DISPLAY_COL_PIN_MASK[col].D;
                }
            }

            lookupTable_[row][pwmCount].B = 
                ( DISPLAY_ALL_COL_PIN_MASK.B & ~colValues.B) + 
                ( DISPLAY_ALL_ROW_PIN_MASK.B & DISPLAY_ROW_PIN_MASK[row].B);

            lookupTable_[row][pwmCount].C = 
                ( DISPLAY_ALL_COL_PIN_MASK.C & ~colValues.C) + 
                ( DISPLAY_ALL_ROW_PIN_MASK.C & DISPLAY_ROW_PIN_MASK[row].C);

            lookupTable_[row][pwmCount].D = 
                ( DISPLAY_ALL_COL_PIN_MASK.D & ~colValues.D) + 
                ( DISPLAY_ALL_ROW_PIN_MASK.D & DISPLAY_ROW_PIN_MASK[row].D);
        }
    }
}

