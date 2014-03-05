#include <Wire.h>
#include <Streaming.h>

enum {BUFFER_SIZE  = 42};
const uint8_t SLAVE_ADDRESS = 1; 


void setup()
{
    Serial.begin(115200);
    Wire.begin();

    Serial << "starting i2c master"  << endl;
}

void loop()
{
    uint8_t buffer[BUFFER_SIZE];
    static int cnt = 0;
    uint8_t numItems;

    if (cnt%2 == 0)
    {
        buffer[0] = 7; // all on cmd
        buffer[1] = 0;
        numItems = 2;
    }
    else
    {
        buffer[0] = 8; // all off cmd
        buffer[1] = 0;
        numItems = 2;
    }

    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(buffer, numItems);
    Wire.endTransmission();
    Serial << "sent cmd" << buffer[0] << endl;

    delay(10);

    buffer[0] = 0; // swap buffer cmd
    buffer[1] = 0;
    numItems = 2;

    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(buffer, numItems);
    Wire.endTransmission();

    Serial << "sent cmd" << buffer[0] << endl;
    Serial << endl;

    delay(1000);


    cnt++;
}
