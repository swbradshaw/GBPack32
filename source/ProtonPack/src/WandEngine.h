#pragma once

#include "Arduino.h"
#include "Observer.h"
#include "EventArgs.h"
#include <Adafruit_NeoPixel.h>
#define FASTLED_INTERNAL // remove annoying pragma messages
#include <FastLED.h>
#include "led/Indicator.h"
#include "led/colors.h"
#include "common.h"
#include "esp_random.h"


class WandEngine : public Observer<EventArgs>
{
    /*
  0 - Slow blo
  1-5 WAND bar
  6 - Wand indiactor
  7 - Vent
  8 - Firing LED?
  9-16 - 8 Barrel LEDs
*/
    // Indicator flashing speeds
    const uint8_t INDICATOR_FAST_FLASH = 100;
    const uint16_t INDICATOR_MEDIUM_FLASH = 500;
    const uint16_t INDICATOR_SLOW_FLASH = 1000;

public:
    WandEngine(Adafruit_NeoPixel &strip);
    void notify(EventArgs args) override;
    void init();
    void work();

private:
    Adafruit_NeoPixel &_wandLeds;
    void setBarrelColorAll(uint8_t red, uint8_t green, uint8_t blue);
    void setBarrelColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue);

    void changeTheme(int themeMode);

    void initFireColors();
    void initPostWandFire();
    void handleFireStream(uint8_t updateInterval);
    void handlePostFireStream(uint8_t updateInterval);

    void updateWandBar();
    void updateWandBarrel();
    void barWandOn(uint8_t idleTime);
    void barWandFireIdle(uint8_t idleTime);
    void barFiring(uint8_t firingSpeedInterval);
    void barBooting(uint8_t bootSp, uint8_t idleTime, bool init);
    void barShutdown(uint8_t shutdownSp, bool init);
    void setBarColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue);
    void setBarColorAll(uint8_t red, uint8_t green, uint8_t blue);
    void setBarPixelOn(uint16_t pixel, CRGB color);

    Indicator _slowBlowIndicator;
    Indicator _wandOnIndicator;
    Indicator _timedFiringIndicator;
    Indicator _ventIndicator;

    uint32_t wandBarColor = 0xFFA500;

    /* Wand Barrel LEDS */
    uint8_t _numBarrelLeds;
    uint8_t **_barrelLedState;

    /* Wand Bargraph LEDS */
    uint8_t _numBarLeds;
    uint8_t **_barLedState;
    int8_t _runningLedTracker;
    bool _reverseSeqTracker;
    int8_t _fireSeqTracker;
    unsigned long _prevBarTime;
    bool _bootState;

    bool packBooting = false;
    bool packOn = false;
    bool packShuttingDown = false;
    bool packVenting = false;
    // wand states
    bool wandBooting = false;
    bool wandOn = false;
    bool wandShuttingDown = false;
    bool wandVentOn = false;
    bool wandFiring = false;
    bool wandFinishedFiring = false;

    colours c_temp_start = C_CUSTOM;
    colours c_temp_effect = C_WHITE;

    uint8_t state = 0;
    uint8_t statePrev = 0;

    unsigned long _prevUpdate;
    unsigned long _interval = 50;  // milliseconds between updates (These are set in the main program)
    unsigned long _lastUpdate = 0; // last update of position


    uint8_t i_barrel_light = 0; // barrel light that is currently animating

    void handleEvent(EventArgs args);
};
