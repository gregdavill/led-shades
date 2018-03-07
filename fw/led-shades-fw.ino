#include "accel.h"
#include "leds.h"

void setup()
{
    ledsInit();
    accelInit();
}

void loop()
{
    if (accelReadIntPin()) {
        accelResetIntLatch();
        ledSwitch();
    }
    ledsUpdate();
}
