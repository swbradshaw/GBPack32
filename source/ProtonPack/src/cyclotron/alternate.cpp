#include "alternate.h"
#include "../common.h"

void Alternate::idle() {
  activePattern = IDLE;
  totalSteps = totalLEDs;
  setInterval();
  fadeStep = 0;
  fadeComplete = false;
  direction = 0;

  for (uint8_t i = 0; i < 41; i++) {
    _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
  }
}

void Alternate::setInterval() {
    if (activePattern == BOOT) {
        interval = 10;
    } else if (activePattern == POWERDOWN) {
        interval = 200;
    } else if (activePattern == IDLE) {
        interval = 1500;
    } else {
        interval = 50;
    }
}

void Alternate::idleUpdate()  {
    int32_t color_1 = color;
    int32_t color_2 = _packLeds.Color(0, 0, 0);
    if (direction == 1) {
        color_2 = color_1;
        color_1 = _packLeds.Color(0, 0, 0);
    }
    for (int8_t i = 0; i < 3; i++) {
        _packLeds.setPixelColor(this->topLeftLEDS[i], color_1);
        _packLeds.setPixelColor(this->bottomRightLEDS[i], color_1);
        _packLeds.setPixelColor(this->topRightLEDS[i], color_2);
        _packLeds.setPixelColor(this->bottomLeftLEDS[i], color_2);
    }
    direction++;
    if (direction > 1) {
        direction = 0;
    }
}

void Alternate::setOverheating(int stage) {
    overHeatStage = stage;
    switch(overHeatStage) {
      case 0:
        setInterval();
        break;
      case 1:
        interval = 250;
        break;
      case 2:
        interval = 100;
        break;
      case 3:
        interval = 50;
        break;
      case 4:
        activePattern = OVERHEATED;
        interval = 180;
        index = 0;
        break;
      case 5:
        vent();
        break;
    }
  }