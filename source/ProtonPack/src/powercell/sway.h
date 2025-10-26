#pragma once

#include "powercell.h"
#include "Arduino.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include <Adafruit_NeoPixel.h>
#include "../led/colors.h"

class Sway : public Powercell {
    public:
        Sway(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color, uint8_t tailSize) : _packLeds(strip), startLED(startLED), endLED(endLED), color(color), tailSize(tailSize) {};
        virtual void init();
        virtual void boot();
        virtual void idle(bool _force);
        virtual void powerDown();
        virtual void vent();
        virtual void packOff();
        virtual bool work();
        virtual void setIsFiring(bool IsFire);
        virtual void notify(EventArgs args);
        virtual void setPercent(float percent);
        virtual void setOverheating(int stage);

        virtual ~Sway() {};

    private:
        Adafruit_NeoPixel &_packLeds;
        const uint8_t startLED;
        const uint8_t endLED;
        uint32_t color = 0;
        uint8_t tailSize = 3;
        uint8_t index = 0;
        uint8_t totalLEDs = 0;
        uint8_t totalSteps = 0;
        uint8_t segmentIndex = 0;
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
