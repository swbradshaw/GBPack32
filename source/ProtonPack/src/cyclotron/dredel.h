#pragma once

#include "ledring.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class Dredel : public LedRing {
    public:

        Dredel(Adafruit_NeoPixel &strip, uint8_t startLED, uint8_t endLED, uint32_t color) : LedRing(strip,startLED,endLED,color) {};

        void idle() override;
        void idleUpdate() override;
        void boot() override;
        void bootUpdate() override;
        void setInterval() override;
        void powerDown() override;
        void powerDownUpdate() override;
        void packOff() override;
        virtual ~Dredel() {};

        void incrementBoot();
        int16_t rampParameter(int16_t param, int16_t ini, int16_t tg, int16_t incr);
        void rotation();
        void idleAnimation();
        void ramp(int16_t tg_updateSp);

        uint8_t mytime;
    private:
        int16_t iniUpSp;
        uint16_t incr_updateSp;
        uint16_t int_updateSp;
        int16_t cycUpdateSp;
        int16_t cycPosTracker;
        int16_t cycBrightness;
        int16_t cycTrail;
        int16_t cycFlash;
        int16_t cycHead;
        unsigned long prevUpdateSpTime = 0;
        uint8_t incrementBy = 1;
        const int16_t BOOT_TIME = 5000;
        const int16_t POWEROFF_TIME = 5000;
        const int16_t BOOT_INTERVAL = 15;
};
