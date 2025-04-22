#pragma once

#include "ledring.h"
#include <Adafruit_NeoPixel.h>

enum  Direction { FORWARD, REVERSE };

// DoubleSway is a class that will create a pattern that will sway back and forth on the top and bottom of the led ring
// implementation is a bit complex because the top and bottom of the led ring are going to be moving in opposite directions
// its further complex because the starting LED index is variable based on user's pack implementation
// these values will need to be passed into the class

class DoubleSway : public LedRing {
    public:

        DoubleSway(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color, uint8_t tailSize, uint8_t topLeft, uint8_t topRight, uint8_t bottomLeft, uint8_t bottomRight)
        : LedRing(strip,startLED,endLED,color), tailSize(tailSize), topLeft(topLeft), topRight(topRight), bottomLeft(bottomLeft), bottomRight(bottomRight) {};

        void idle() override;
        void boot() override;
        void bootUpdate() override;
        void idleUpdate() override;
        void increment() override;
        void setInterval() override;
        void powerDown() override;
        virtual ~DoubleSway() {};


    private:
        uint8_t tailSize = 3;
        uint8_t indexBottom = -1;

        // led ring leds run counter clockwise

        // top line is going to decrement from left to right
        uint8_t topLeft = 0;
        uint8_t topRight = 21;
        Direction topDirection;

        // bottom line is going to increment from left to right
        uint8_t bottomLeft = topLeft+1;
        uint8_t bottomRight = topRight+1;
        Direction bottomDirection;

};
