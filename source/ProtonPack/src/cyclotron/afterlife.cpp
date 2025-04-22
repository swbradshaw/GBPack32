#include "afterlife.h"
#include "../common.h"

void AfterlifeRing::setInterval() {
    if (activePattern == BOOT) {
        interval = 60;
    } else {
        interval = 25;
    }
}

void AfterlifeRing::boot() {
    cycPosTracker = 0;
    activePattern = BOOT;
    indexArray = 0;
    index = 0;
    interval = 60;

    incrementBy = 1;
    iniUpSp = interval;
    cycBrightness = 0;
    cycTrail = 0;
    cycFlash = 0;
    cycHead = 4;
    // Increments to rampup cycloton speeds and brightness
    incr_updateSp = 1;
    // Time intervals to rampup cycloton speeds and brightness
    int_updateSp = BOOT_TIME / abs(BOOT_INTERVAL - iniUpSp);
    clear();
}

void AfterlifeRing::bootUpdate() {
    // start really slow and get faster and faster to a max speed
    ramp(BOOT_INTERVAL);
}

void AfterlifeRing::powerDown() {
    cycBrightness = 0;
    cycTrail = 0;
    cycFlash = 0;
    cycHead = 4;
    // Increments to rampup cycloton speeds and brightness
    iniUpSp = interval;
    incr_updateSp = 1;
    // Time intervals to rampup cycloton speeds and brightness
    int_updateSp = POWEROFF_TIME / abs(60 - iniUpSp);
    activePattern = POWERDOWN;
}

void AfterlifeRing::powerDownUpdate() {
    ramp(60);
}

void AfterlifeRing::idle() {
    activePattern = IDLE;
    setInterval();
    clear();
    incrementBy = 4;
    iniUpSp = 60;
    cycBrightness = 0;
    cycTrail = 0;
    cycFlash = 0;
    cycHead = 4;
    // Increments to rampup cycloton speeds and brightness
    incr_updateSp = 1;
    // Time intervals to rampup cycloton speeds and brightness
    int_updateSp = 3000 / abs(25 - iniUpSp);
}

void AfterlifeRing::idleUpdate() {
    ramp(20);
}

void AfterlifeRing::packOff() {
    activePattern = OFF;
    clear();
    _packLeds.show();
}

void AfterlifeRing::ramp(int16_t tg_updateSp)
{
    rotation();
    cycPosTracker += 1; //incrementBy;
    if (cycPosTracker > visibleLEDcount - 1)
    {
        cycPosTracker = 0;
    }

     // Ramp cyclotron UPDATE SPEED
    if (millis() - prevUpdateSpTime >= int_updateSp)
    {
        prevUpdateSpTime = millis();
        interval = rampParameter(interval, iniUpSp, tg_updateSp, incr_updateSp);
        // Pre March 2025 code where ramping up the speed would also cause it to skip LEDs, making it animate faster
        // switch (interval) {
        //     case 45:
        //         incrementBy = 1;
        //         break;
        //     case 40:
        //         incrementBy = 2;
        //         break;
        //     case 30:
        //         incrementBy = 3;
        //         break;
        //     case 20:
        //         incrementBy = 4;
        //         break;
        //     case 15:
        //         incrementBy = 5;
        //         break;
        //     default:
        //         break;
        // }
    }
}


int16_t AfterlifeRing::rampParameter(int16_t param, int16_t ini, int16_t tg, int16_t incr) {
    if ((tg - ini) >= 0) { // Ramp UP
        if (param + incr <= tg) {
            param = param + incr;
        } else {
            param = tg;
        }
    }
    else { // Ramp DOWN
        if (incr >= param || param - incr < tg) {
            param = tg;
        } else {
            param = param - incr;
            if (param < 0)
            {
                param = 0;
            }
        }
    }
    return param;
}


void AfterlifeRing::rotation() {
    uint16_t factor = 1000 * (60 - interval) / (60 - 1);
    cycBrightness = min(255, 100 + (255 * factor) / 1000);
    // cycHead = (4 * factor) / 1000;
    // cycFlash = (6 * factor) / 1000;
    // cycTrail = (5 * factor) / 1000;

    for (int8_t i = (cycPosTracker - cycTrail); i < (cycPosTracker - cycTrail + visibleLEDcount); i++)
    {
        // Correct i between 0 to _numleds
        int8_t j = i;
        if (i < 0) {
            j = visibleLEDcount + i;
        } else if (i > visibleLEDcount - 1) {
            j = i - visibleLEDcount;
        }

        //  Trail pixels
        if (i < cycPosTracker)
        {
            uint8_t redTail = min(255, (cycBrightness / 3) * ((cycTrail + 1) - (cycPosTracker - i)) / (cycTrail + 1));
            _packLeds.setPixelColor(visibleLEDS[j], redTail, redTail / 25, 0);
        }
        // Flash pixels
        else if (i < cycPosTracker + cycFlash) {
            uint8_t flashBlue = 0;
            if (interval < 10) {
                flashBlue = 1;
            }
            else if (interval < 20) {
                flashBlue = 1;
            }
            _packLeds.setPixelColor(visibleLEDS[j], cycBrightness, cycBrightness / 5, flashBlue);
        }
        // Head pixels
        else if (i < cycPosTracker + cycFlash + cycHead) {
            uint8_t redHead = min(255, (cycBrightness / 5) * ((cycHead + 1) - (i - (cycPosTracker + cycFlash))) / (cycHead + 1));
            _packLeds.setPixelColor(visibleLEDS[j], redHead, redHead / 25, 0);
        }
        // All others pixel are OFF
        else {
            _packLeds.setPixelColor(visibleLEDS[j], 0,0,0);
        }
    }

}



