#pragma once

#include "powercell.h"
#include "Arduino.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include <FastLED.h>
#include "../led/colors.h"
#include <Adafruit_NeoPixel.h>

// SnowPowercell will create a pattern that looks like snow falling. pixels fall at different speeds and colors
// this class could be overridden to create a Matrix theme, or rewritten to pass in custom colors to make that work

class SnowPowercell : public Powercell {
    public:
        SnowPowercell(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color) : _packLeds(strip), startLED(startLED), endLED(endLED), color(color) {};
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

        virtual ~SnowPowercell() {};

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
        // how fast this snow falls, in terms of cycles
        const int snowFlakeSpeed[10] = {2, 1, 5, 4, 6, 2, 5, 3, 3, 6};
        // color of this snow
        const int snowFlakeColor[10] = {170, 250, 170, 240, 165, 255, 160, 235, 175, 255};

        int totalFlakes = 5;
        // which flake setting is this using (for speed and color)
        int snowSelection[5];
        // current LED position of this snow (up to 5 flakes)
        int snowPosition[5];
        // current timing of this flake (is it time to move yet?) - this gets substracted every cycle
        int snowCounter[5];
        // which flake is next in the queue? (0-9)
        int currentSnowConfigCounter = 0;

        bool bootComplete = false;
        bool isFiring = false;
        unsigned long interval = 75;
        unsigned long lastUpdate;
        Powercell_Pattern  activePattern;
        Powercell_Direction direction;
        void increment();
        void clear();
        void setInterval();
        void initFlakes();

        void virtual bootUpdate();
        void virtual idleUpdate();
        void virtual powerDownUpdate();
        void virtual ventUpdate();

};
