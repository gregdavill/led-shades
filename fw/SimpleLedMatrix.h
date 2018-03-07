#ifndef SIMPLE_LED_MATRIX__H_
#define SIMPLE_LED_MATRIX__H_

#include "Arduino.h"
#include "FastLED.h"
#include "Print.h"
#include "hw_1_00.h"

#include "Adafruit_GFX.h"

class SimpleLedMatrix : public Adafruit_GFX {
protected:
public:
    SimpleLedMatrix(uint16_t w, uint16_t h);

    // Required Non-Transaction
    void drawPixel(int16_t x, int16_t y, uint32_t color);

    uint32_t colorPack(CRGB colour);

protected:
    CRGB leds[NUM_LEDS];
};

#endif
