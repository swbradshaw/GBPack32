#pragma once

// unused class - this is a stub for the future
#include <Arduino.h>
#include "./theme/PackTheme.h"

class Context
{
  public:
    Context();
    PackTheme getTheme();

  private:
    int currentTheme = 0;


};