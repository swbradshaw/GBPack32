#include "SmokeEngine.h"
#include "common.h"

SmokeEngine::SmokeEngine(uint8_t pin,
    const uint32_t *maxOnTime,
    const uint32_t *minOffTime) : pin(pin), ptr_MAX_ON_TIME(maxOnTime), ptr_MIN_OFF_TIME(minOffTime)
{
    smokeStopTime = 0;
    smokeStartTime = 0;
    smokeOn = false;
}

void SmokeEngine::init()
{
    if (pin > -1) {
        hasSmoke = true;
        pinMode(pin, OUTPUT);
        smokeOFF();
    } else {
        hasSmoke = false;
    }
}

void SmokeEngine::work()
{
    if (hasSmoke && smokeOn) {
        if ((smokeDuration > 0) && ((millis() - smokeStartTime) >= smokeDuration)) {
            smokeDuration = 0;
            smokeOFF();
        }
        if (millis() - smokeStartTime >= *ptr_MAX_ON_TIME) {
            smokeDuration = 0;
            smokeOFF();
        }
    }
}

void SmokeEngine::startSmoke(uint16_t duration){
    if (hasSmoke) {
        if (!smokeOn) {
            smokeDuration = duration;
            smokeON();
        }
    }
}

void SmokeEngine::smokeON() {
    if (hasSmoke) {
        if (!smokeOn && ((millis() - smokeStopTime) >= *ptr_MIN_OFF_TIME)) {
            debugln("SmokeEngine: Smoke ON");
            digitalWrite(pin, HIGH);
            smokeStartTime = millis();
            smokeOn = true;
        }
    }
}

void SmokeEngine::smokeOFF()
{
    if (hasSmoke && smokeOn) {
        debugln("SmokeEngine: Smoke OFF");
        digitalWrite(pin, LOW);
        smokeStopTime = millis();
        smokeOn = false;
        sendSmokeOffEvent();
    }
}

void SmokeEngine::handleEvent(EventArgs args) {
    if (args.eventName == EVENT_SMOKE_ON) {
        startSmoke(args.eventDetail1.toInt());
    }
}

void SmokeEngine::handleOverheatEvent(EventArgs args) {
    int stage = args.eventDetail1.toInt();
    switch(stage) {
        case 0:
            // no overheating
            smokeOFF();
            break;
        case 4:
            // pack has overheated; start the smoke in preparation for venting
            startSmoke(6000);
            break;
        default:
            break;
    }
}

void SmokeEngine::sendSmokeOffEvent() {
    EventArgs smomeOffEvent;
    smomeOffEvent.eventName = EVENT_SMOKE_OFF;
    Subject::notify(smomeOffEvent);
}

void SmokeEngine::notify(EventArgs args) {
    if (args.eventName.startsWith("EVENT_SMOKE_")) {
        debugln("SmokeEngine: " + args.eventName);
        handleEvent(args);
    }
    if (args.eventName == EVENT_OVERHEAT) {
        handleOverheatEvent(args);
    }
}