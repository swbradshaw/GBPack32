#pragma once

#include <Arduino.h>
#include "Observer.h"
#include "EventArgs.h"

class PackTheme : public Observer<EventArgs>
{
  public:
    PackTheme();
    void notify(EventArgs args) override;
    void setTheme(int themeNumber);

  private:
    void handleEvent(EventArgs args);
    void loadTheme();
    int currentTheme = 0;
    String themeFolder;


};