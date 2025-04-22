#include "Indicator.h"
#include "FastLED.h"

Indicator::Indicator(Adafruit_NeoPixel &strip, uint8_t pixel)
    : _strip(strip), _pixel(pixel)
{
    _prevTime = 0;
    _flashingState = false;
    _pulse = false;
}
CRGB Indicator::Orange(){
    return CRGB(255,100,0);
}
CRGB Indicator::Green(){
    return CRGB(173, 255, 90);
}
CRGB Indicator::Yellow(){
    return CRGB(255, 200, 0);
}
CRGB Indicator::Red(){
    return CRGB(255, 0, 0);
}
CRGB Indicator::White(){
    return CRGB(150, 150, 150);
}

void Indicator::begin() { clear(); }

void Indicator::setColor(uint32_t color)
{
    _strip.setPixelColor(_pixel, color);
}

void Indicator::setColor(CRGB color)
{
    setColor(color[0], color[1], color[2]);
}
void Indicator::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    _strip.setPixelColor(_pixel, _strip.gamma8(red), _strip.gamma8(green), _strip.gamma8(blue));
}

void Indicator::show()
{
    _strip.show();
}

void Indicator::clear()
{
    setColor(0, 0, 0);
}

void Indicator::update(CRGB color, uint16_t updateSp) // flashing
{
  if (updateSp > 0)
    {
        if (!_flashingState)
        {
            _pulse = false;
            _flashingState = true;
        }
        if ((millis() - _prevTime) >= updateSp)
        {
            _prevTime = millis();
            if (_pulse == true)
            {
            setColor(color[0], color[1], color[2]);
            _pulse = false;
            }
            else
            {
                clear();
                _pulse = true;
            }
        }
    }
    else
    {        
        setColor(color[0], color[1], color[2]);
        _flashingState = 0;
    }
}

