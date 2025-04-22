#pragma once

#include "Arduino.h"
#include "Observer.h"
#include "Subject.h"
#include "EventArgs.h"

class SmokeEngine : public Observer<EventArgs>,  public Subject<EventArgs>
{

    public:
        SmokeEngine(uint8_t pin, const uint32_t *maxOnTime, const uint32_t *minOffTime);
        void init();
        void work();
        void notify(EventArgs args) override;

    private:
        bool hasSmoke = false;
        bool smokeOn = false;
        uint8_t pin;
        unsigned long smokeStartTime = 0;
        unsigned long smokeStopTime = 0;
        const uint32_t *ptr_MAX_ON_TIME;
        const uint32_t *ptr_MIN_OFF_TIME;
        uint16_t smokeDuration;

        void handleEvent(EventArgs args);
        void startSmoke(uint16_t duration);
        void smokeON();
        void smokeOFF();
        void sendSmokeOffEvent();
        void handleOverheatEvent(EventArgs args);
};
