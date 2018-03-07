#include "leds.h"
#include <Fonts/Org_01.h>
#include "SimpleLedMatrix.h"

SimpleLedMatrix slm(25, 8);

void LinePatternWave();
void ScrollingText();
void ScrollingCheverons();
void ExplodingCircles();
void ParticleTest();

void (*functions[])(void) = {
    LinePatternWave,
    ScrollingText,
    ScrollingCheverons,
    ParticleTest,
};

uint functionIndex = 0;

void (*pUpdateFunction)(void);

static CRGB randomColour;

void ledsInit()
{
    FastLED.clear();
    FastLED.show();

    FastLED.setBrightness(16);
    // FastLED.setDither(0);

    functionIndex = 0;
    ledSwitch();
}

void ledSwitch()
{
    if (++functionIndex >= (sizeof(functions) / sizeof(void (*)(void)))) {
        functionIndex = 0;
    }
    pUpdateFunction = functions[functionIndex];

    randomColour = ColorFromPalette(RainbowColors_p, rand(), 255, LINEARBLEND);
}

void ledsUpdate() { pUpdateFunction(); }

const char *textString = "Happy New Year!";

void ScrollingText()
{
    // Turn the LED on, then pause
    static int count = 0;
    static int colourIndex = 30;

    slm.setFont(&Org_01);
    slm.setTextWrap(false);

    int16_t x = 0;
    int16_t y = 0;
    uint16_t w = 0;
    uint16_t h = 0;

    slm.getTextBounds((char *)textString, 0, 0, &x, &y, &w, &h);

    uint16_t maxScroll = (x + w + slm.width());

    if (count > maxScroll) {
        count = 0;
        colourIndex = rand();
    }

    slm.setTextColor(slm.colorPack(ColorFromPalette(RainbowColors_p, colourIndex, 255, LINEARBLEND)), 0);
    slm.setCursor(slm.width() - count, 5);

    slm.print((char *)textString);

    FastLED.show();

    FastLED.delay(60);

    FastLED.clear();

    count++;
}

void LinePatternWave()
{
    // Turn the LED on, then pause
    static int count = 0;
    static int colourIndex = 0;

    for (int y = 0; y < slm.height(); y++) {
        for (int x = 0; x < slm.width(); x++) {
            //    CRGB colour = ColorFromPalette(RainbowColors_p, colourIndex, 255, LINEARBLEND);
            CRGB colour = randomColour;

            /* fade colour based on pixel location. */
            uint32_t fadeAmount = ((x + count + y) % 5);
            if (fadeAmount == 4) {
                colour = CRGB::Black;
            }
            if (fadeAmount == 3) {
                colour = CRGB::Black;
            }
            if (fadeAmount == 2) {
                colour.fadeLightBy(180);
            }
            if (fadeAmount == 1) {
                colour.fadeLightBy(120);
            }

            slm.drawPixel(x, y, slm.colorPack(colour));
        }
    }
    FastLED.show();

    FastLED.delay(80);

    FastLED.clear();

    count++;

    colourIndex++;
}

void drawCheveron(int x, int y, uint32_t colour, bool direction)
{
    if (direction) {
        slm.drawLine(x - 5, 0 - y, x, -5 - y, colour);
        slm.drawLine(x + 5, 0 - y, x, -5 - y, colour);
        y -= 1;
        slm.drawLine(x - 5, 0 - y, x, -5 - y, colour);
        slm.drawLine(x + 5, 0 - y, x, -5 - y, colour);
    }
    else {
        slm.drawLine(x - 5, y + 5, x, y, colour);
        slm.drawLine(x + 5, y + 5, x, y, colour);
        y -= 1;
        slm.drawLine(x - 5, y + 5, x, y, colour);
        slm.drawLine(x + 5, y + 5, x, y, colour);
    }
}

void ScrollingCheverons()
{
    // Turn the LED on, then pause
    static int count = 0;
    static int colourIndex = 0;

    int offset = count % 5;

    //    CRGB colour = ColorFromPalette(CloudColors_p, colourIndex, 255, LINEARBLEND);
    CRGB colour = randomColour;

    bool direction = true;

    drawCheveron(5, offset, slm.colorPack(colour), direction);
    offset -= 5;
    drawCheveron(5, offset, slm.colorPack(colour), direction);
    offset -= 5;
    drawCheveron(5, offset, slm.colorPack(colour), direction);
    offset -= 5;
    drawCheveron(5, offset, slm.colorPack(colour), direction);

    offset = count % 5;

    drawCheveron(19, offset, slm.colorPack(colour), direction);
    offset -= 5;
    drawCheveron(19, offset, slm.colorPack(colour), direction);
    offset -= 5;
    drawCheveron(19, offset, slm.colorPack(colour), direction);
    offset -= 5;
    drawCheveron(19, offset, slm.colorPack(colour), direction);

    FastLED.show();

    FastLED.delay(80);

    FastLED.clear();

    count++;

    colourIndex++;
}

void ExplodingCircles()
{
    // Turn the LED on, then pause
    static int count = 0;
    static int colourIndex = 0;

    int offset = count % 15;

    CRGB colour;

    if (offset > 5 && offset < 15) {
        colour = ColorFromPalette(RainbowColors_p, offset * 2, 255, LINEARBLEND);
        slm.drawCircle(5, 4, offset - 5, slm.colorPack(colour));
    }

    if (offset > 1 && offset < 11) {
        colour = ColorFromPalette(LavaColors_p, offset * 2, 255, LINEARBLEND);
        slm.drawCircle(18, 4, offset - 1, slm.colorPack(colour));
    }

    if (offset > 4 && offset < 13) {
        colour = ColorFromPalette(PartyColors_p, offset * 2, 255, LINEARBLEND);
        slm.drawCircle(18, 3, offset - 4, slm.colorPack(colour));
    }

    if (offset < 10) {
        colour = ColorFromPalette(LavaColors_p, offset * 2, 255, LINEARBLEND);
        slm.drawCircle(5, 4, offset, slm.colorPack(colour));
    }
    FastLED.show();

    FastLED.delay(80);

    FastLED.clear();

    count++;

    colourIndex++;
}

typedef struct particle {
    int x;
    int y;
    int r;
    CRGB colour;
    bool alive = false;
} particle_t;

void ParticleTest()
{
    // Turn the LED on, then pause
    static int count = 0;
    static int colourIndex = 0;

    static particle_t particles[4];

    for (int i = 0; i < sizeof(particles) / sizeof(particle_t); i++) {
        particle_t *p = &particles[i];

        if (p->alive) {
            if (p->r > 8) {
                p->alive = false;
                continue;
            }

            slm.drawCircle(p->x, p->y, p->r, slm.colorPack(p->colour));

            p->colour.fadeToBlackBy(32);
            p->r++;
        }
        else {
            /* Should this be alive? */
            if (rand() % 20 == 0) {
                p->alive = true;
                p->x = 2 + (rand() % 20);
                p->y = 2 + (rand() % 4);
                p->r = 0;
                p->colour = ColorFromPalette(RainbowColors_p, rand(), 255, LINEARBLEND);
            }
        }
    }

    FastLED.show();

    FastLED.delay(50);

    FastLED.clear();

    count++;

    colourIndex++;
}
