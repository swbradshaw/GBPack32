#include "snowfall.h"
#include "../common.h"

void SnowPowercell::init() {
    totalLEDs = endLED - startLED + 1;
    debugln("[Snowfall] init()- start/end, color:   "+String(startLED)+"/"+String(endLED)+ " , "+String(color));
}

void SnowPowercell::notify(EventArgs args) {}
void SnowPowercell::setPercent(float percent) {}

bool SnowPowercell::work() {
     if ((millis() - lastUpdate) > interval) {
        lastUpdate = millis();
        switch (activePattern) {
            case PC_BOOT:
                bootUpdate();
                break;
            case PC_IDLE:
                idleUpdate();
                break;
            case PC_POWERDOWN:
                powerDownUpdate();
                break;
            default:
                return false;
                break;
        }
        return true;
   }
   return false;
}

void SnowPowercell::packOff() {
    activePattern = PC_OFF;
}

void SnowPowercell::boot() {
    // initialize the LED count
    totalLEDs = endLED - startLED + 1;
    bootComplete = false;
    // boot up the powercell
    activePattern = PC_BOOT;
    segmentIndex = 0;
    totalSteps = endLED;
    index = endLED;
    direction = PC_REVERSE;
    setInterval();
}

void SnowPowercell::bootUpdate() {
    // update the powercell during boot
    for (uint8_t i = startLED; i < endLED; i++) {
        if (i == index) {
          _packLeds.setPixelColor(i, color);
        } else {
          for (uint8_t j = startLED + segmentIndex; j < startLED + totalLEDs; j++) {
            if (j != index ) {
              _packLeds.setPixelColor(j, _packLeds.Color(0, 0, 0));
            }
          }
        }
    }
    increment();
}

void SnowPowercell::setIsFiring(bool IsFire) {
    isFiring = IsFire;
    setInterval();
}

void SnowPowercell::setOverheating(int stage) {
  overHeatStage = stage;
  switch(overHeatStage) {
    case 0:
      setInterval();
      break;
    case 1:
      interval = 50;
      break;
    case 2:
      interval = 30;
      break;
    case 3:
    case 4:
      interval = 10;
      break;
    case 5:
      vent();
      break;
  }

}

void SnowPowercell::idle(bool force) {
  // don't force idle state until the boot animation is completed
  if (bootComplete || force) {
    activePattern = PC_IDLE;
    initFlakes();
    setInterval();

    totalSteps = endLED;
    index = startLED;
  }
}
void SnowPowercell::initFlakes() {
  currentSnowConfigCounter = 0;
  for (int i = 0; i < totalFlakes; i++) {
    snowSelection[i] = currentSnowConfigCounter;
    snowPosition[i] = endLED;
    snowCounter[i] = snowFlakeSpeed[snowSelection[i]];
    currentSnowConfigCounter++;
  }
}

void SnowPowercell::idleUpdate() {
    // clear out the leds
    // clear();
    // update the snow
    for (int i = 0; i < totalFlakes; i++) {
      if (snowPosition[i] >= startLED) {
        if (snowCounter[i] == 0) {
          // move the snowflake
          _packLeds.setPixelColor(snowPosition[i], _packLeds.Color(0, 0, 0));
          snowPosition[i]--;
          if (snowPosition[i] >= startLED) {
            if (snowFlakeColor[snowSelection[i]] > 200) {
              _packLeds.setPixelColor(snowPosition[i], _packLeds.Color(snowFlakeColor[snowSelection[i]],snowFlakeColor[snowSelection[i]],snowFlakeColor[snowSelection[i]], 150));
            } else {
              _packLeds.setPixelColor(snowPosition[i], Wheel(snowFlakeColor[snowSelection[i]]));
            }
          }
          snowCounter[i] = snowFlakeSpeed[snowSelection[i]];
        } else {
          snowCounter[i]--;
        }
      } else {
        // debugln("color just finished" + String(snowFlakeColor[snowSelection[i]]));
        // reset the snowflake
        snowPosition[i] = endLED;
        snowSelection[i] = currentSnowConfigCounter;
        snowCounter[i] = snowFlakeSpeed[snowSelection[i]];
        currentSnowConfigCounter++;
        if (currentSnowConfigCounter > 9) {
          currentSnowConfigCounter = 0;
        }
      }
    }

}

void SnowPowercell::setInterval() {
    if (isFiring) {
        interval = 10;
    } else {
        interval = 15;
    }
}

/*
    Increment the Powercell LED state
*/
void SnowPowercell::increment() {
    if (activePattern == PC_BOOT) {
        --index;
        if (index - segmentIndex < startLED) {
            index = totalSteps - 1;
            segmentIndex++;
            if (segmentIndex == totalLEDs) {
                // boot complete, let state to idle
                bootComplete = true;
                idle(false);
                clear();
            }
        }
    } else if (activePattern == PC_IDLE) {
        // snow!
        // ++index;
        // if (index > endLED) {
        //     index = startLED;
        // }
    }
}

void SnowPowercell::powerDown() {
    activePattern = PC_POWERDOWN;
}

void SnowPowercell::powerDownUpdate() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, DimColor(_packLeds.getPixelColor(i)));
    }
}

void SnowPowercell::vent() {
    activePattern = PC_VENT;
    clear();
    _packLeds.show();
}
void SnowPowercell::ventUpdate() {

}

// clear all the powercell leds
void SnowPowercell::clear()
{
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
    }
}
