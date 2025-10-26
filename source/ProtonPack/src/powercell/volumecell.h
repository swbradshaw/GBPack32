#pragma once

#include "powercell.h"
#include "Arduino.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include "../led/colors.h"
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

class VolumeCell : public Powercell {
    public:
        VolumeCell(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color) : _packLeds(strip), startLED(startLED), endLED(endLED), color(color) {};
        virtual void init();
        virtual void boot();
        virtual void idle(bool force);
        virtual void powerDown();
        virtual void vent();
        virtual void packOff();
        virtual bool work();
        virtual void setIsFiring(bool IsFire);
        virtual void notify(EventArgs args);
        virtual void setPercent(float percent);
        virtual void setOverheating(int stage);

        virtual ~VolumeCell() {};

    private:
        Adafruit_NeoPixel &_packLeds;
        const uint8_t startLED;
        const uint8_t endLED;
        uint32_t color = 0;
        uint8_t totalLEDs = 0;
        float multiplier = 1.0f;
        float volumeMeterDivider = 0.0f;
        unsigned long interval = 25;
        unsigned long lastUpdate;
        Powercell_Pattern  activePattern;

        void clear();
        void draw();
        uint8_t nextHeight = 0;
        uint8_t currentHeight = 0;

};
