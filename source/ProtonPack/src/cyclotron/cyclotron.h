#pragma once

#include "Arduino.h"

class Cyclotron {
    public:
        virtual void init() = 0;
        virtual bool work() = 0;
        virtual void setCustomValue(int name, String value) = 0;
        // state changes
        virtual void boot() = 0;
        virtual void idle() = 0;
        virtual void powerDown() = 0;
        virtual void vent() = 0;
        virtual void packOff() = 0;
        // overheating stage 0 (none) - 4 (overheated),  5 (venting)
        virtual void setOverheating(int stage) = 0;
        // set the cyclotron percentage (0-100) - not used in every theme
        virtual void setPercent(float percent) = 0;
        virtual void setIsFiring(bool IsFire) = 0;

        virtual ~Cyclotron() {};

};