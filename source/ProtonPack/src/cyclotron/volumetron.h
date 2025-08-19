#pragma once

#include "cyclotron.h"
#include "Arduino.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include "../led/colors.h"

class Volumetron : public Cyclotron {
    public:
        Volumetron(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color) : _packLeds(strip), startLED(startLED), endLED(endLED), color(color) {};
        virtual void init();
        virtual void boot();
        virtual void idle();
        virtual void powerDown();
        virtual void vent();
        virtual void packOff();
        virtual bool work();
        virtual void setIsFiring(bool IsFire);
        virtual void setCustomValue(int name, String value);
        virtual void setOverheating(int stage);
        virtual void setPercent(float percent);
        virtual ~Volumetron() {};
        void clear();

        Adafruit_NeoPixel &_packLeds;
        const uint8_t startLED;
        const uint8_t endLED;
        uint32_t color;
        uint8_t totalLEDs = 0;
        bool isFiring = false;
        // array of colors
        uint32_t colors[4] = { 42, 170, 85, 255 };

        enum  Pattern { OFF, IDLE, BOOT, POWERDOWN, OVERHEATED, VENT };
        unsigned long interval = 25;
        unsigned long lastColorChange = 0;
        byte colorIndex = 0;
        Pattern activePattern;
        String customValues[2];
        uint8_t nextBrightness = 0;
        uint8_t currentBrightness = 0;

        unsigned long lastUpdate;


    private:
        void initLeds();

};
