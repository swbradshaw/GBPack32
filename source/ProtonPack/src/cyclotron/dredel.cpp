#include "dredel.h"
#include "../common.h"

void Dredel::setInterval() {
    interval = 25;
}

void Dredel::boot() {
    cycPosTracker = 0;
    activePattern = BOOT;
    indexArray = 0;
    index = 0;
    interval = 25;

    incrementBy = 1;
    iniUpSp = interval;
    // Increments to rampup cycloton speeds and brightness
    incr_updateSp = 1;
    // Time intervals to rampup cycloton speeds and brightness
    int_updateSp = 500;
}

void Dredel::bootUpdate() {
    // start really slow and get faster and faster to a max speed
    ramp(BOOT_INTERVAL);
}


void Dredel::powerDown() {
    // Increments to rampup cycloton speeds and brightness
    iniUpSp = 25;//interval;
    incr_updateSp = 1;
    // Time intervals to rampup cycloton speeds and brightness
    int_updateSp = POWEROFF_TIME / 35;
    activePattern = POWERDOWN;
}

void Dredel::powerDownUpdate() {
    ramp(60);
}

void Dredel::idle() {
    activePattern = IDLE;
    setInterval();

    incrementBy = 5;
    iniUpSp = 60;
    // Increments to rampup cycloton speeds and brightness
    incr_updateSp = 1;
    // Time intervals to rampup cycloton speeds and brightness
    int_updateSp = 100;
}

void Dredel::idleUpdate() {
    ramp(60);
    if (interval == 60) {
        incrementBy = 5;
        interval = 25;
    }
}

void Dredel::packOff() {
    activePattern = OFF;
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
    }
}

void Dredel::ramp(int16_t tg_updateSp)
{
    rotation();
    cycPosTracker += incrementBy;
    if (cycPosTracker > totalLEDs - 1)
    {
        cycPosTracker = 0;
    }

     // Ramp cyclotron UPDATE SPEED
    if (millis() - prevUpdateSpTime >= int_updateSp)
    {
        prevUpdateSpTime = millis();
        interval = rampParameter(interval, iniUpSp, tg_updateSp, incr_updateSp);
        switch (interval) {
            case 45:
                incrementBy = 1;
                break;
            case 40:
                incrementBy = 2;
                break;
            case 30:
                incrementBy = 3;
                break;
            case 20:
                incrementBy = 4;
                break;
            case 15:
                incrementBy = 5;
                break;
            default:
                break;
        }
    }
}


int16_t Dredel::rampParameter(int16_t param, int16_t ini, int16_t tg, int16_t incr) {
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


void Dredel::rotation() {
    uint16_t factor = 1000 * (60 - interval) / (60 - 1);
    cycBrightness = min(255, 100 + (255 * factor) / 1000);
    cycHead = (4 * factor) / 1000;
    cycFlash = (6 * factor) / 1000;
    cycTrail = (25 * factor) / 1000;

    for (int8_t i = (cycPosTracker - cycTrail); i < (cycPosTracker - cycTrail + totalLEDs); i++)
    {
        // Correct i between 0 to _numleds
        int8_t j = i;
        if (i < 0) {
            j = totalLEDs + i;
        } else if (i > totalLEDs - 1) {
            j = i - totalLEDs;
        }

        //  Trail pixels
        if (i < cycPosTracker)
        {
            uint8_t blueTail = min(255, (cycBrightness / 3) * ((cycTrail + 1) - (cycPosTracker - i)) / (cycTrail + 1));
            _packLeds.setPixelColor(startLED+j, 0, blueTail / 25, blueTail);
        }
        // Head pixels
        else if (i < cycPosTracker + cycFlash + cycHead) {
            uint8_t blueHead = min(255, (cycBrightness / 5) * ((cycHead + 1) - (i - (cycPosTracker + cycFlash))) / (cycHead + 1));
            _packLeds.setPixelColor(startLED+j, 0, blueHead / 25, blueHead);
        }
        // All others pixel are OFF
        else {
            _packLeds.setPixelColor(startLED+j, 0,0,0);
        }
    }

}



