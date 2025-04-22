#pragma once

#include "ledring.h"
#include <Adafruit_NeoPixel.h>

class SlimeRing : public LedRing {
    public:

        SlimeRing(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color) : LedRing(strip,startLED,endLED,color) {};

        void idleUpdate() override;
        virtual ~SlimeRing() {};


    private:

};
