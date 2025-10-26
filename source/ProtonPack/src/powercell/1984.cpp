#include "1984.h"
#include "../common.h"

void EightyFour::init() {
    totalLEDs = endLED - startLED + 1;
    debugln("[1984] init()- start/end, color:   "+String(startLED)+"/"+String(endLED)+ " , "+String(color));
}

void EightyFour::notify(EventArgs args) {}
void EightyFour::setPercent(float percent) {}

bool EightyFour::work() {
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
            case PC_VENT:
                ventUpdate();
                break;
            default:
                return false;
                break;
        }
        return true;
   }
   return false;
}

void EightyFour::packOff() {
    activePattern = PC_OFF;
}

void EightyFour::boot() {
    // initialize the LED count
    debugln("1984: boot");
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

void EightyFour::bootUpdate() {
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

void EightyFour::setIsFiring(bool IsFire) {

}

void EightyFour::setOverheating(int stage) {
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

void EightyFour::idle(bool force = false) {
  // don't force idle state until the boot animation is completed
  if (bootComplete || force) {
    activePattern = PC_IDLE;
    direction = PC_FORWARD;
    setInterval();

    totalSteps = endLED;
    index = startLED;
  }
}

void EightyFour::idleUpdate() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        if (i == index) {
          _packLeds.setPixelColor(i, color);
        }
        if (index == endLED+1) {
            clear();
        }
      }
      increment();
}

void EightyFour::setInterval() {
    if (activePattern == PC_BOOT) {
        interval = 30;
    } else {
        interval = 75;
    }
}

/*
    Increment the Powercell LED state
*/
void EightyFour::increment() {
    if (activePattern != PC_POWERDOWN) {
        if (direction == PC_FORWARD) {
          index++;
          if (index > totalSteps+1) {
            index = startLED;
          }
        } else { // Direction == REVERSE
          --index;
          if (activePattern == PC_BOOT) {
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
          }
        }
    } else {
        index++;
        if (index >= totalSteps) {
          index = startLED;
        }
    }
}

void EightyFour::powerDown() {
    activePattern = PC_POWERDOWN;
    direction = PC_REVERSE;
    setInterval();
    index = startLED;
}

void EightyFour::powerDownUpdate() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, DimColor(_packLeds.getPixelColor(i)));
    }
}

void EightyFour::vent() {
    activePattern = PC_VENT;
    direction = PC_FORWARD;
    setInterval();
    index = startLED;
    clear();
    _packLeds.show();
}
void EightyFour::ventUpdate() {
    // for (uint8_t i = startLED; i < endLED+1; i++) {
    //     _packLeds.setPixelColor(i, color);
    // }
}

// clear all the powercell leds
void EightyFour::clear()
{
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
    }
}
