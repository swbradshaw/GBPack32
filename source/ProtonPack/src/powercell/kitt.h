#pragma once

#include "powercell.h"
#include "Arduino.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include "../led/colors.h"
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

// KittPowercell will create a red pattern that mimick's the sentry mode of the 80's KITT car.
// In addition this can generate an orange speech animation to mimick KITT talking
// This is done by listening for events EVENT_AUDIO_PLAY_SOUND_CLIP and EVENT_AUDIO_STOP_SOUND_CLIP

class KittPowercell : public Powercell {
    public:
        KittPowercell(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color, uint8_t tailSize) : _packLeds(strip), startLED(startLED), endLED(endLED), color(color), tailSize(tailSize) {};
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

        virtual ~KittPowercell() {};

    private:
        Adafruit_NeoPixel &_packLeds;
        const uint8_t startLED;
        const uint8_t endLED;
        uint32_t color = 0;
        uint32_t speechColor = 16737280; // orange
        uint32_t startColor = 0;
        bool speaking = false;
        uint8_t tailSize = 3;
        uint8_t index = 0;
        uint8_t totalLEDs = 0;
        uint8_t totalSteps = 0;
        uint8_t segmentIndex = 0;
        uint8_t centerIndex = 0;
        uint8_t speechIndex = 0;
        const int speechDistance[10] = {3, 1, 6, 1, 4, 2, 6, 2, 4, 1};

        unsigned long interval = 75;
        unsigned long lastUpdate;
        Powercell_Pattern  activePattern;
        Powercell_Direction direction;
        void increment();
        void clear();
        void setInterval();
        void speechUpdate();
        void incrementSpeech();


        void virtual bootUpdate();
        void virtual idleUpdate();
        void virtual powerDownUpdate();
        void virtual ventUpdate();

};
