#include <Wire.h>
#include <Streaming.h>
#include "i2cmaster.h"
#include "panels_g4.h"


void speedTest();
void displayColTest();
void displayRowTest();
void displayPixelTest();
void displayOnAndOffTest();

void setup()
{
    pinMode(A2,OUTPUT);
    //Wire.begin();
    i2c_init();   
    //TWBR = 50;
    //TWBR = 4;
    //TWBR = 2;  // 1MHz
    TWBR = 17;   // 400kHz
}

void loop()
{
    speedTest();
    //displayColTest();
    //displayRowTest();
    //displayPixelTest();
    //displayOnAndOffTest();
}


// Test functions
// ----------------------------------------------------------------------------


void speedTest()
{
    while(1)
    {
        const uint8_t numGrayScaleValues = 2;
        static bool isFirst = true;
        static uint8_t row = 0;
        static I2CMessageBuffer msgArray[DISPLAY_MATRIX_SIZE];

        if (isFirst)
        {
            for (uint8_t i=0; i<DISPLAY_MATRIX_SIZE; i++)
            {
                DisplayMatrix  matrix;
                matrix.setNumGrayScaleValues(numGrayScaleValues);
                matrix.setAllZero();
                matrix.setCol(i,numGrayScaleValues-1);
                matrix.getMsgFromData(msgArray[i]);
            }
            isFirst = false;
        }

        for (uint8_t slaveInd=0; slaveInd < 1; slaveInd++) //I2C_NUM_SLAVES; slaveInd++)
        {
            uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd]; 
            //Wire.beginTransmission(slaveAddress);
            //Wire.write(msgArray[row].getDataArray(), msgArray[row].getLength());
            //Wire.write(msgArray[row].getDataArray(), 20);
            //Wire.endTransmission();

            uint8_t (&data)[I2C_BUFFER_SIZE] = msgArray[row].getDataArray();
            uint8_t dataLength = msgArray[row].getLength();
            i2c_start((slaveAddress << 1) + I2C_WRITE);     
            //for (uint8_t i=0; i< dataLength; i++)
            for (uint8_t i=0; i<50; i++) //dataLength; i++)
            {
                //i2c_write(data[i]);
                i2c_write(i);
            }
            i2c_stop();                                   
        }

        delay(500);
        //delayMicroseconds(6500);
        digitalWrite(A2,HIGH);
        delayMicroseconds(200);
        digitalWrite(A2,LOW);
        delayMicroseconds(1000);

        row = (row+1)%DISPLAY_MATRIX_SIZE;
    }
}


void displayColTest()
{
    const uint8_t numGrayScaleValues = 7;
    static uint8_t col=0;
    DisplayMatrix matrix;
    I2CMessageBuffer msg;
    matrix.setNumGrayScaleValues(numGrayScaleValues);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        //uint8_t slaveAddress = I2C_SLAVE_ADDRESS[I2C_NUM_SLAVES - slaveInd -1];
        matrix.setAllZero();
        if ((col < DISPLAY_MATRIX_SIZE) && ((slaveInd==0)||(slaveInd==2)))
        {
            matrix.setCol(col,numGrayScaleValues-1);
        }
        if ((col >= DISPLAY_MATRIX_SIZE) && ((slaveInd==1)||(slaveInd==3)))
        {
            matrix.setCol(col%DISPLAY_MATRIX_SIZE,numGrayScaleValues-1);
        }
        msg = matrix.getMsgFromData();
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();
    }

    delayMicroseconds(500);
    digitalWrite(A2,HIGH);
    delayMicroseconds(50);
    digitalWrite(A2,LOW);
    delayMicroseconds(50);

    col++;
    if (col >= 2*DISPLAY_MATRIX_SIZE)
    {
        col=0;
    }
    
}

void displayRowTest()
{
    const uint8_t numGrayScaleValues = 6;
    static uint8_t row=0;
    I2CMessageBuffer msg;;
    DisplayMatrix matrix;
    matrix.setNumGrayScaleValues(numGrayScaleValues);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        matrix.setAllZero();
        if ((row < DISPLAY_MATRIX_SIZE) && (slaveInd<2))
        {
            matrix.setRow(row,numGrayScaleValues-1);
        }
        if ((row >= DISPLAY_MATRIX_SIZE) && (slaveInd>=2))
        {
            matrix.setRow(row%DISPLAY_MATRIX_SIZE,numGrayScaleValues-1);
        }
        msg = matrix.getMsgFromData();

        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();
    }

    delay(100);
    //delayMicroseconds(10000);
    digitalWrite(A2,HIGH);
    delayMicroseconds(500);
    digitalWrite(A2,LOW);
    delayMicroseconds(500);

    row++;
    if (row >= 2*DISPLAY_MATRIX_SIZE)
    {
        row=0;
    }

}

void displayPixelTest()
{ 
    static uint8_t row = 0;
    static uint8_t col = 0;
    const uint8_t numGrayScaleValues = 8;

    DisplayMatrix matrix;
    I2CMessageBuffer msg;
    matrix.setNumGrayScaleValues(numGrayScaleValues);
    matrix.setAllZero();
    matrix.setPixel(row,col,numGrayScaleValues-1);
    //matrix.setPixel(row,col,1);

    for (uint8_t slaveInd=0; slaveInd<I2C_NUM_SLAVES; slaveInd++)
    {
        uint8_t slaveAddress = I2C_SLAVE_ADDRESS[slaveInd];
        msg = matrix.getMsgFromData();
        Wire.beginTransmission(slaveAddress);
        Wire.write(msg.getDataArray(), msg.getLength());
        Wire.endTransmission();
    }

    delay(5);
    digitalWrite(A2,HIGH);
    delayMicroseconds(100);
    digitalWrite(A2,LOW);
    delay(1);

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

    //delayMicroseconds(3333);
}

//void displayOnAndOffTest()
//{
//    static int cnt = 0;
//    uint8_t numItems;
//    I2CMessageBuffer msg;
//    uint8_t cmd;
//
//    if (cnt%2 == 0)
//    {
//        cmd = CMD_SET_DISPLAY_ALL_ON;
//    }
//    else
//    {
//        cmd = CMD_SET_DISPLAY_ALL_OFF;
//    }
//    msg.setByte(0,cmd);
//    msg.setByte(1,0);
//    msg.setLength(2);
//
//    Wire.beginTransmission(SLAVE_ADDRESS);
//    Wire.write(msg.getDataArray(), msg.getLength());
//    Wire.endTransmission();
//
//    delay(10);
//    digitalWrite(A2,HIGH);
//    delayMicroseconds(100);
//    digitalWrite(A2,LOW);
//    delay(20);
//
//    cnt++;
//    delay(100);
//}
