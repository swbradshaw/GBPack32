#pragma once

#include <Arduino.h>
#include "../common.h"
#include "Subject.h"
#include <PinButton.h>
#include "EventArgs.h"

class ButtonReader : public Subject<EventArgs>
{
  public:
    ButtonReader(byte fireGPIO, byte packGPIO, byte wandGPIO, byte ventGPIO, byte specialGPIO);
    void work();

  private:

    PinButton* fireButton;
    PinButton* packButton;
    PinButton* wandButton;
    PinButton* ventButton;
    PinButton* musicButton;

    bool started = false;
    long startTime = 0;
    long lastRead = 0;
    int lastFireClick = 0;
    int lastSpecialClick = 0;
    void readButtons();
};
