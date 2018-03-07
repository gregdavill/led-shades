#include "accel.h"
#include <Arduino.h>
#include <Wire.h>

void accelInit()
{
    uint8_t buffer[2] = {0x21, 0x01};

    Wire.begin();

    buffer[0] = 0x20;
    buffer[1] = 0x04;
    Wire.beginTransmission(0x18);
    Wire.write(buffer, 2); /* Active high INT pins */
    Wire.endTransmission();

    buffer[0] = 0x1B;
    buffer[1] = 0x20;
    Wire.beginTransmission(0x18);
    Wire.write(buffer, 2); /* Active Double Tap on INT2 */
    Wire.endTransmission();

    buffer[0] = 0x16;
    buffer[1] = 0x20;
    Wire.beginTransmission(0x18);
    Wire.write(buffer, 2); /* Enable Double Tap feature */
    Wire.endTransmission();

    buffer[0] = 0x1E;
    buffer[1] = 0x00;
    Wire.beginTransmission(0x18);
    Wire.write(buffer, 2); /* Enable filtered data to tap detection */
    Wire.endTransmission();

    buffer[0] = 0x2A;
    buffer[1] = 0xC7;
    Wire.beginTransmission(0x18);
    Wire.write(buffer, 2); /* Set Tap detection params */
    Wire.endTransmission();

    buffer[0] = 0x0F;
    buffer[1] = 0x03;
    Wire.beginTransmission(0x18);
    Wire.write(buffer, 2); /* Set 2G range */
    Wire.endTransmission();

    accelResetIntLatch();

    pinMode(11, INPUT_PULLUP);
}

void accelResetIntLatch()
{
    return;
    uint8_t buffer[2] = {0x21, 0x80};

    Wire.beginTransmission(0x18);
    Wire.write(buffer, 2);
    Wire.endTransmission();
}

bool accelReadIntPin()
{
    uint8_t buffer[1] = {0x09};
    Wire.beginTransmission(0x18);
    Wire.write(buffer, 1);
    Wire.endTransmission();
    Wire.requestFrom(0x18, 3);
    uint8_t data = Wire.read();
    uint8_t extrainfo = Wire.read();
    uint8_t tapDir = Wire.read();

    static uint32_t timeout = 0;

    /* Check a register and if we have not recently changed modes change. */
    if (tapDir & 0x20) {
        if ((millis() - timeout) > 500) {
            timeout = millis();
            return 1;
        }
    }

    return 0;
}