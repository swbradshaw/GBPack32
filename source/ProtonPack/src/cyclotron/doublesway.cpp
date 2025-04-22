#include "doublesway.h"
#include "../common.h"


void DoubleSway::boot() {
  activePattern = BOOT;
  totalSteps = totalLEDs;
  setInterval();
  index = topLeft+startLED;
  indexBottom = bottomLeft+startLED;
  topDirection = REVERSE;
  bottomDirection = FORWARD;
  fadeStep = 0;
  fadeComplete = false;
}

void DoubleSway::bootUpdate() {
    idleUpdate();
}

void DoubleSway::idle() {
  topDirection = REVERSE;
  bottomDirection = FORWARD;
  activePattern = IDLE;
  totalSteps = totalLEDs;
  index = topLeft+startLED;
  indexBottom = bottomLeft+startLED;
  setInterval();
  fadeStep = 0;
  fadeComplete = false;
}

void DoubleSway::powerDown() {
    activePattern = POWERDOWN;
    setInterval();
    index = 0;
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, color);
    }
}


void DoubleSway::setInterval() {
    if (activePattern == BOOT) {
        interval = 10;
    } else if (activePattern == POWERDOWN) {
        interval = 200;
    } else {
        interval = 35;
    }
}

void DoubleSway::increment() {

     if (topDirection == FORWARD) {
        index++;
        if (index > endLED) {
            index = startLED;
        } else if (index == startLED+topLeft) {
            topDirection = REVERSE;
        }
    } else { // Direction == REVERSE
        index--;
        if (index < startLED) {
            index = endLED;
        } else if (index == endLED-(totalLEDs-topRight)) {
            topDirection = FORWARD;
        }
    }

    if (bottomDirection == FORWARD) {
        indexBottom++;
        if (indexBottom > endLED) {
            indexBottom = startLED;
        } else if (indexBottom == startLED+bottomRight) {
            bottomDirection = REVERSE;
        }
    } else { // Direction == REVERSE
        indexBottom--;
        if (indexBottom < startLED) {
            indexBottom = endLED;
        } else if (indexBottom == endLED-(totalLEDs-bottomLeft)) {
            bottomDirection = FORWARD;
        }
    }

}

void DoubleSway::idleUpdate()  {
     for (int8_t i = startLED; i < endLED+1; i++) {
        if (i == index || (i == indexBottom)) {
            _packLeds.setPixelColor(i, color);
        } else {
            if (abs(i - index) < tailSize) {
                _packLeds.setPixelColor(i, DimColor(_packLeds.getPixelColor(i)));
            } else if (abs(i - indexBottom) < tailSize) {
                _packLeds.setPixelColor(i, DimColor(_packLeds.getPixelColor(i)));
            } else {
                _packLeds.setPixelColor(i, 0);
            }
        }
    }
    increment();
}
