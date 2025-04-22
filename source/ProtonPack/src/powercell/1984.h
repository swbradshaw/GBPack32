#pragma once

#include "powercell.h"
#include "Arduino.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include "../led/colors.h"
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

class EightyFour : public Powercell {
    public:
        EightyFour(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color) : _packLeds(strip), startLED(startLED), endLED(endLED), color(color) {};
        virtual void init();
        virtual void boot();
        virtual void idle(bool force);
        virtual void powerDown();
        virtual void vent();
        virtual void packOff();
        virtual bool work();
        virtual void setIsFiring(bool IsFire);
        virtual void notify(EventArgs args);
        virtual void setOverheating(int stage);

        virtual ~EightyFour() {};

    private:
        Adafruit_NeoPixel &_packLeds;
        const uint8_t startLED;
        const uint8_t endLED;
        uint32_t color = 0;
        uint8_t index = 0;
        uint8_t totalLEDs = 0;
        uint8_t totalSteps = 0;
        uint8_t segmentIndex = 0;
        uint8_t overHeatStage = 0;
        bool bootComplete = false;
        unsigned long interval = 75;
        unsigned long lastUpdate;
        Powercell_Pattern  activePattern;
        Powercell_Direction direction;
        void increment();
        void clear();
        void setInterval();


        void virtual bootUpdate();
        void virtual idleUpdate();
        void virtual powerDownUpdate();
        void virtual ventUpdate();

};
