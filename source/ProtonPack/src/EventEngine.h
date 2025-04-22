#pragma once

#define FASTLED_INTERNAL // remove annoying pragma messages
#include <Arduino.h>
#include "Observer.h"
#include "Subject.h"
#include "EventArgs.h"
#include "common.h"

class EventEngine : public Observer<EventArgs>,  public Subject<EventArgs>
{
  public:
    EventEngine();
	  void notify(EventArgs args) override;
    void work();
    void setTheme(int theme);
    void handleWatchEvent(EventArgs event);

  private:
    unsigned long lastUpdate = 0;

    // pack states
    bool packBooting = false;
    bool packOn = false;
    bool packShuttingDown = false;
    // wand states
    bool wandBooting = false;
    bool wandOn = false;
    bool wandFiring = false;
    bool wandShuttingDown = false;
    bool wandVentOn = false;

    void handleEvent(EventArgs args);
    void setState(uint8_t newState);
    void handleButtonEvent(EventArgs args);
    void handleWandButtonEvent(EventArgs args);
    void handlePackButtonEvent(EventArgs args);
    void handleFireButtonEvent(EventArgs args);
    void handleAudioEvent(EventArgs args);
    void handleThemeChangeEvent(EventArgs args);
    void handleFollowUpEvent(String eventName);
    void handleWandFireButtonStateChange(bool released);
    void handleWandButtonStateChange();
    void handlePackButtonStateChange();
    bool isPackOn();
    bool isWandOn();


};