#pragma once

#include "EventArgs.h"

enum  Powercell_Pattern { PC_OFF, PC_BOOT, PC_IDLE, PC_POWERDOWN, PC_VENT };
enum  Powercell_Direction { PC_FORWARD, PC_REVERSE };

class Powercell {
    public:
        virtual void init() = 0;
        virtual bool work() = 0;
        // state changes
        virtual void boot() = 0;
        virtual void idle(bool force) = 0;
        virtual void powerDown() = 0;
        virtual void vent() = 0;
        virtual void packOff() = 0;
        virtual void setIsFiring(bool IsFire) = 0;
        // overheating stage 0 (none) - 4 (overheated),  5 (venting)
        virtual void setOverheating(int stage) = 0;
        // set the powercell percentage (0-100) - not used in every theme
        virtual void setPercent(float percent);
        virtual void notify(EventArgs args);

        virtual ~Powercell() {};

};