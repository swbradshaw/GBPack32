#pragma once

#include "ledring.h"
#include <Adafruit_NeoPixel.h>

class ChristmasRing : public LedRing {
    public:

        ChristmasRing(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color) : LedRing(strip,startLED,endLED,color) {};

        void increment() override;
        virtual ~ChristmasRing() {};

    private:

};
