#include "volumetron.h"
#include "../common.h"


void Volumetron::setPercent(float percent) {
  // recevies a value 0-100 of the LED brightness
  nextBrightness = (percent > 75.0f) ? 255 : 100+ (uint8_t)(percent * 1.5f); // convert to 0-255
  if (nextBrightness > 255) {
    nextBrightness = 255;
  }
  if  (percent > 50.0f) {
   // debugln("Volumetron: setPercent() - color change?");
   // but only if its recent?
  }

}

void Volumetron::init() {
    totalLEDs = endLED - startLED + 1;
    debugln("Volumetron: init "+String(startLED)+" "+String(endLED));
    activePattern = OFF;
    initLeds();
    _packLeds.show();
}

bool Volumetron::work() {
     if ((millis() - lastUpdate) > interval) {
        lastUpdate = millis();
        if (nextBrightness != currentBrightness) {
          currentBrightness = nextBrightness;
          // debugln("Volumetron: work() - currentBrightness: " + String(currentBrightness));
          //_packLeds.setBrightness(currentBrightness);
          if (currentBrightness > 180) {
            if (lastColorChange < millis() - 3000) {
                lastColorChange = millis();
                colorIndex++;
                if (colorIndex >= 4) colorIndex = 0;
                color = colors[colorIndex];
            }
          }
          initLeds();
          return true;
        }
   }
   return false;
}


void Volumetron::initLeds() {
    if (currentBrightness > 125) {
      _packLeds.setBrightness(255);
      for (uint8_t i = startLED; i < endLED+1; i++) {
          _packLeds.setPixelColor(i, Wheel(color));
      }
    } else {
      clear();
    }
}


void Volumetron::setOverheating(int stage) {

}

void Volumetron::setCustomValue(int name, String value){
    customValues[name] = value;
}

void Volumetron::packOff() {
    activePattern = OFF;
}

void Volumetron::boot() {
    activePattern = BOOT;
}

void Volumetron::idle() {
  activePattern = IDLE;
}

void Volumetron::setIsFiring(bool IsFire) {
    isFiring = IsFire;
}

void Volumetron::powerDown() {
    activePattern = POWERDOWN;
}

void Volumetron::vent() {
    activePattern = VENT;
}

void Volumetron::clear() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
    }
}
