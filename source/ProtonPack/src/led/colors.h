#pragma once

/**
 * Colours are based on the RGB pattern; for GRB the Red/Green values should be switched.
 * Brightness is based on varying the intensity of each (0-255) using a relative value.
 *
 * However, colours based on Hue use a colour degree, a Saturation, and Value (brightness).
*/
#include "Arduino.h"
#include <FastLED.h>

enum colours {
  C_BLACK,
  C_WHITE,
  C_WARM_WHITE,
  C_PINK,
  C_PASTEL_PINK,
  C_RED,
  C_LIGHT_RED,
  C_RED2,
  C_RED3,
  C_RED4,
  C_RED5,
  C_ORANGE,
  C_BEIGE,
  C_YELLOW,
  C_CHARTREUSE,
  C_GREEN,
  C_DARK_GREEN,
  C_MINT,
  C_AQUA,
  C_LIGHT_BLUE,
  C_MID_BLUE,
  C_NAVY_BLUE,
  C_BLUE,
  C_PURPLE,
  C_CUSTOM,
};

extern CHSV getHue(uint8_t i_colour, uint8_t i_brightness, uint8_t i_saturation);
extern CRGB getHueAsRGB(uint8_t i_colour, uint8_t i_brightness, bool b_grb);
extern CRGB getHueAsGRB(uint8_t i_colour, uint8_t i_brightness);
extern CRGB getHueColour(uint8_t i_colour, uint8_t i_brightness);



inline uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
inline uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

// Returns the Red component of a 32-bit color
inline uint8_t Red(uint32_t color)
{
  return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
inline uint8_t Green(uint32_t color)
{
  return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
inline uint8_t Blue(uint32_t color)
{
  return color & 0xFF;
}

inline uint32_t DimColor(uint32_t color)
{
  // Shift R, G and B components one bit to the right
  uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
  return dimColor;
}
inline uint32_t DimColorByAmount(uint32_t color, byte dimAmount)
{
  // Shift R, G and B components one bit to the right
  uint32_t dimColor = Color(Red(color) >> dimAmount, Green(color) >> dimAmount, Blue(color) >> dimAmount);
  return dimColor;
}