#pragma once

#include "Arduino.h"
#include "Observer.h"
#include "Subject.h"
#include "EventArgs.h"
#include "led/colors.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include <Adafruit_NeoPixel.h>
#include <millisDelay.h>
#include "common.h"
#include "./powercell/powercell.h"
#include "./powercell/1984.h"
#include "./powercell/kitt.h"
#include "./powercell/snowfall.h"
#include "./powercell/volumecell.h"

#include "./cyclotron/cyclotron.h"
#include "./cyclotron/ledring.h"
#include "./cyclotron/slimering.h"
#include "./cyclotron/afterlife.h"
#include "./cyclotron/dredel.h"
#include "./cyclotron/christmas.h"
#include "./cyclotron/doublesway.h"
#include "./cyclotron/alternate.h"
#include "./cyclotron/volumetron.h"


class PackEngine : public Observer<EventArgs>,  public Subject<EventArgs>
{

public:
    PackEngine(Adafruit_NeoPixel &strip);
    void notify(EventArgs args) override;
    void init();
    void work();

private:

    // START - ugly hack job to hardcode pixel positions that are visible in my pack
    uint8_t topLeftLeds[4] = {1, 0, 39, 38}; // 8 7 6 5 is 1pm
    uint8_t topRightLeds[4] = {8, 9,10,11};//{38, 37, 36, 35, 34, 33, 32, 31, 30, 29};
    uint8_t bottomLeftLeds[4] = {28, 29, 30, 31};
    uint8_t bottomRightLeds[4] = {18, 19, 20, 21};//{28, 27, 26, 25, 24, 23, 22, 21, 20, 19};

    // starting from top left, going clockwise, these are the 16 leds that are visible
    uint8_t ledRingVisibleLeds[16] = {38,39,0,1,  8,9,10,11,  18,19,20,21,  28,29,30,31};

    // END - ugly hack job to hardcode pixel positions that are visible

    Adafruit_NeoPixel &_packLeds;

    millisDelay delayTimer;

    /* Pack Vent LEDS */
    uint8_t numVentLeds;

    /* Pack States */
    bool packBooting = false;
    bool packOn = false;
    bool packShuttingDown = false;
    bool packVenting = false;
    bool ventLightOn = false;
    bool wandVentOn = false;
    bool wandFiring = false;

    bool ventSwitchTogglesVent = false;
    bool sendVolumeEvents = false;

    unsigned long wandFiringStart = 0;
    uint8_t lastOverheatStage = 0; // 1 -faster lights, 2 - even faster ligths, 3 = faster lights, stream + beeping, 4 - only beeping, 5 - start venting

    byte smokeColor = 150;

    Powercell *powercell;
    Cyclotron *cyclotron;

    void handleEvent(EventArgs args);
    void handleAudioEvent(EventArgs args);
    void handleOverheatEvent(EventArgs args);
    void enableVentLights(bool OnorOff);
    void changeTheme(int themeMode);

    void handleOverheat();
    uint32_t getOverheatTime();
    uint8_t getOverheatStage(unsigned long timeSinceStart);
    void sendOverheatEvent();
};
