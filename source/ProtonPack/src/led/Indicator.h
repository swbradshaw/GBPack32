#pragma once

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "FastLED.h"

class Indicator
{
public:
    Indicator(Adafruit_NeoPixel &strip, uint8_t pixel);
    // static methods for COLORS
    CRGB Orange();
    CRGB Green();
    CRGB Yellow();
    CRGB Red();
    CRGB White();
    void setColor(uint32_t color);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(CRGB color);
    void begin();
    void show();
    void clear();
    void update(CRGB color, uint16_t updateSpeed);

private:
    Adafruit_NeoPixel &_strip;
    unsigned long _prevTime;
    uint8_t _pixel;
    bool _flashingState;
    bool _pulse;
};
