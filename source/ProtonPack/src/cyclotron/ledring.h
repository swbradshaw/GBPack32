#pragma once

#include "cyclotron.h"
#include "Arduino.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include "../led/colors.h"

enum CUSTOM_VALUES { FADE_SETTING };

class LedRing : public Cyclotron {
    public:
        LedRing(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color) : _packLeds(strip), startLED(startLED), endLED(endLED), color(color) {};
        virtual void init();
        virtual void boot();
        virtual void idle();
        virtual void powerDown();
        virtual void vent();
        virtual void packOff();
        virtual bool work();
        virtual void setIsFiring(bool IsFire);
        virtual void setCustomValue(int name, String value);
        virtual void increment();
        virtual void setOverheating(int stage);
        virtual ~LedRing() {};
        void clear();

        Adafruit_NeoPixel &_packLeds;
        const uint8_t startLED;
        const uint8_t endLED;
        uint32_t color;
        uint8_t index = 0;
        uint8_t totalLEDs = 0;
        bool isFiring = false;
        // FIXME - this is hardcoded to 40
        uint16_t indexArray;
        const int ledArray[40] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        enum  Pattern { OFF, IDLE, BOOT, POWERDOWN, OVERHEATED, VENT };
        unsigned long interval = 75;
        Pattern activePattern;
        String customValues[2];
        uint8_t totalSteps = 0;
        unsigned long lastUpdate;
        bool fadeComplete = false;
        int fadeStep = 0;

        uint8_t overHeatStage = 0;

    private:

        void virtual setInterval();
        void virtual bootUpdate();
        void virtual idleUpdate();
        void virtual powerDownUpdate();
        void virtual ventUpdate();
        void overheated();
        void overheatUpdate();

        void fade(byte redStartValue, byte redEndValue, byte greenStartValue, byte greenEndValue, byte blueStartValue, byte blueEndValue, int totalSteps, int startLED, int endLED);

};
