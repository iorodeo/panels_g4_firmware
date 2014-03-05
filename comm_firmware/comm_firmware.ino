#include <Wire.h>
#include <Streaming.h>
#include "panels_g4.h"

const uint8_t SLAVE_ADDRESS = 1; 

void displayColTest();
void displayRowTest();
void displayPixelTest();
void displayOnAndOffTest();

void setup()
{
    Wire.begin();
}

void loop()
{
    displayColTest();
    //displayRowTest();
    //displayPixelTest();
    //displayOnAndOffTest();
}


// Test functions
// ----------------------------------------------------------------------------

void displayColTest()
{
    static uint8_t col=0;
    DisplayMatrix matrix;
    I2CMessageBuffer msg;
    matrix.setNumGrayScaleValues(MAX_NUM_GRAYSCALE_VALUES);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        matrix.setAllZero();
        if ((col < DISPLAY_MATRIX_SIZE) && ((slaveInd==0)||(slaveInd==2)))
        {
            matrix.setCol(col,MAX_NUM_GRAYSCALE_VALUES-1);
        }
        if ((col >= DISPLAY_MATRIX_SIZE) && ((slaveInd==1)||(slaveInd==3)))
        {
            matrix.setCol(col%DISPLAY_MATRIX_SIZE,MAX_NUM_GRAYSCALE_VALUES-1);
        }
        msg = matrix.getMsgFromData();
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();

    }

    delay(5);
    msg.setByte(0,CMD_SWAP_DISPLAY_BUFFER);
    msg.setByte(1,0);
    msg.setLength(2);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();
    }

    col++;
    if (col >= 2*DISPLAY_MATRIX_SIZE)
    {
        col=0;
    }
    delay(10);
}

void displayRowTest()
{
    static uint8_t row=0;
    DisplayMatrix matrix;
    I2CMessageBuffer msg;
    matrix.setNumGrayScaleValues(MAX_NUM_GRAYSCALE_VALUES);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        matrix.setAllZero();
        if ((row < DISPLAY_MATRIX_SIZE) && (slaveInd<2))
        {
            matrix.setRow(row,MAX_NUM_GRAYSCALE_VALUES-1);
        }
        if ((row >= DISPLAY_MATRIX_SIZE) && (slaveInd>=2))
        {
            matrix.setRow(row%DISPLAY_MATRIX_SIZE,MAX_NUM_GRAYSCALE_VALUES-1);
        }
        msg = matrix.getMsgFromData();
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();
    }

    msg.setByte(0,CMD_SWAP_DISPLAY_BUFFER);
    msg.setByte(1,0);
    msg.setLength(2);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();
    }

    row++;
    if (row >= 2*DISPLAY_MATRIX_SIZE)
    {
        row=0;
    }
    delay(5);

}

void displayPixelTest()
{ 
    static uint8_t row = 0;
    static uint8_t col = 0;

    DisplayMatrix matrix;
    I2CMessageBuffer msg;
    matrix.setNumGrayScaleValues(MAX_NUM_GRAYSCALE_VALUES);
    matrix.setAllZero();
    matrix.setPixel(row,col,MAX_NUM_GRAYSCALE_VALUES-1);
    //matrix.setPixel(row,col,1);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        msg = matrix.getMsgFromData();
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();
    }

    msg.setByte(0,CMD_SWAP_DISPLAY_BUFFER);
    msg.setByte(1,0);
    msg.setLength(2);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();
    }

    row++;
    if (row >= DISPLAY_MATRIX_SIZE)
    {
        row=0;
        col++;
        if (col >= DISPLAY_MATRIX_SIZE) 
        {
            col = 0;
        }
    }   

    delay(50);
    //delayMicroseconds(3333);
}

void displayOnAndOffTest()
{
    static int cnt = 0;
    uint8_t numItems;
    I2CMessageBuffer msg;
    uint8_t cmd;

    if (cnt%2 == 0)
    {
        cmd = CMD_SET_DISPLAY_ALL_ON;
    }
    else
    {
        cmd = CMD_SET_DISPLAY_ALL_OFF;
    }
    msg.setByte(0,cmd);
    msg.setByte(1,0);
    msg.setLength(2);

    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(msg.getDataArray(), msg.getLength());
    Wire.endTransmission();

    delay(10);

    msg.setByte(0,CMD_SWAP_DISPLAY_BUFFER);
    msg.setByte(1,0);
    msg.setLength(2);

    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(msg.getDataArray(), msg.getLength());
    Wire.endTransmission();

    cnt++;
    delay(1000);
}
