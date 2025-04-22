#include "sway.h"
#include "../common.h"

void Sway::init() {
    totalLEDs = endLED - startLED + 1;
    debugln("[Sway] init()- start/end, color:   "+String(startLED)+"/"+String(endLED)+ " , "+String(color));
}

bool Sway::work() {
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

void Sway::notify(EventArgs args) {
}

void Sway::packOff() {
    activePattern = PC_OFF;
}

void Sway::setOverheating(int stage) {
     // not implemented
}

void Sway::boot() {
    // initialize the LED count
    totalLEDs = endLED - startLED + 1;
    // boot up the powercell
    activePattern = PC_BOOT;
    segmentIndex = 0;
    totalSteps = endLED;
    index = endLED;
    direction = PC_REVERSE;
    setInterval();
}

void Sway::bootUpdate() {
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

void Sway::setIsFiring(bool IsFire) {

}

void Sway::idle(bool _force) {
    activePattern = PC_IDLE;
    direction = PC_FORWARD;
    setInterval();

    totalSteps = endLED;
    index = startLED;
}

void Sway::idleUpdate() {
    for (int8_t i = startLED; i < endLED+1; i++) {
        if (i == index) {
            _packLeds.setPixelColor(i, color);
        } else {
            if (abs(i - index) < tailSize+1) {
                _packLeds.setPixelColor(i, DimColorByAmount(_packLeds.getPixelColor(i),2));
            } else {
                _packLeds.setPixelColor(i, 0);
            }
        }
    }
    increment();
}

void Sway::setInterval() {
    if (activePattern == PC_BOOT) {
        interval = 15;
    } else {
        interval = 55;
    }
}

/*
    Increment the Powercell LED state
*/
void Sway::increment() {
    if (activePattern != PC_POWERDOWN) {
        if (direction == PC_FORWARD) {
          index++;
          if (index > endLED+1) {
            index = endLED-1;
            direction = PC_REVERSE;
          }
        } else { // Direction == REVERSE
          --index;
          if (activePattern == PC_BOOT) {
            if (index - segmentIndex < startLED) {
              index = totalSteps - 1;
              segmentIndex++;
              if (segmentIndex == totalLEDs) {
                // boot complete, let state to idle
                idle(false);
                clear();
              }
            }
          } else {
            if (index < startLED) {
                index = startLED;
                direction = PC_FORWARD;
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

void Sway::powerDown() {
    activePattern = PC_POWERDOWN;
    direction = PC_REVERSE;
    setInterval();
    index = startLED;
}

void Sway::powerDownUpdate() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, DimColor(_packLeds.getPixelColor(i)));
    }
}

void Sway::vent() {
    activePattern = PC_VENT;
    direction = PC_FORWARD;
    setInterval();
    index = startLED;
}
void Sway::ventUpdate() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, color);
    }
}

// clear all the powercell leds
void Sway::clear()
{
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
    }
}

