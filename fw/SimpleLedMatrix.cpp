/***************************************************
  This is our library for generic SPI TFT Displays with
  address windows and 16 bit color (e.g. ILI9341, HX8357D, ST7735...)

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "SimpleLedMatrix.h"
#include <limits.h>

#include "FastLED.h"
#include "hw_1_00.h"

SimpleLedMatrix::SimpleLedMatrix(uint16_t w, uint16_t h) : Adafruit_GFX(w, h) { FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS); }

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint32_t SimpleLedMatrix::colorPack(CRGB colour) { return ((uint32_t)colour.r << 16 | (uint32_t)colour.g << 8 | (uint32_t)colour.b); }

/*
 * Transaction API
 * */

void SimpleLedMatrix::drawPixel(int16_t x, int16_t y, uint32_t color)
{
    CRGB colour = CRGB::Cyan;

    // Unpack 32 bit.
    colour.r = ((color & 0x00FF0000) >> 16);
    colour.g = ((color & 0x0000FF00) >> 8);
    colour.b = ((color & 0x000000FF) >> 0);

    if (x < 0 || y < 0) {
        return;
    }

    if (x > 24 || y > 8) {
        return;
    }

    /* check bounds */
    switch (y) {
        case 0:
            if (x != 0 && x < 24) leds[x - 1] = colour;
            break;

        case 1:
            if (x != 0 && x < 24) leds[46 - (x)] = colour;
            break;

        case 2:
            if (x != 0 && x < 24) leds[45 + (x)] = colour;
            break;

        case 3:
            if (x < 25) leds[93 - (x)] = colour;
            break;

        case 4:
            if (x < 11)
                leds[94 + (x)] = colour;
            else if (x > 13 && x < 25)
                leds[91 + (x)] = colour;
            break;

        case 5:
            if (x < 10)
                leds[135 - (x)] = colour;
            else if (x > 14 && x < 25)
                leds[140 - (x)] = colour;
            break;

        case 6:
            if (x < 10)
                leds[136 + (x)] = colour;
            else if (x > 14 && x < 25)
                leds[131 + (x)] = colour;
            break;

        case 7:
            if (x > 0 && x < 9)
                leds[172 - (x)] = colour;
            else if (x > 15 && x < 24)
                leds[179 - (x)] = colour;
            break;

        default:
            break;
    }
}
