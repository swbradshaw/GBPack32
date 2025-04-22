#pragma once

#include "ledring.h"
#include <Adafruit_NeoPixel.h>
#include "../common.h"

// Alternate is a class that will create a pattern that will alternate between top/left and bottom/right, and then top/right, bottom/left of the led ring
// its complex because the starting LED index is variable based on user's pack implementation
// these values will need to be passed into the class

class Alternate : public LedRing {
    public:

        Alternate(Adafruit_NeoPixel &strip, uint8_t topLeft[], uint8_t topRight[], uint8_t bottomLeft[], uint8_t bottomRight[], uint32_t color)
            : LedRing(strip, 0, 40, color) {
            this->topLeftLEDS = topLeft;
            this->topRightLEDS = topRight;
            this->bottomLeftLEDS = bottomLeft;
            this->bottomRightLEDS = bottomRight;
        }

        void idle() override;
        void idleUpdate() override;
        void setInterval() override;
        void setOverheating(int stage) override;
        virtual ~Alternate() {};


    private:

        uint8_t *topLeftLEDS;
        uint8_t *topRightLEDS;
        uint8_t *bottomLeftLEDS;
        uint8_t *bottomRightLEDS;

        byte direction = 0;

};
