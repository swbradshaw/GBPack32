#include "volumecell.h"
#include "../common.h"

void VolumeCell::init() {
    totalLEDs = endLED - startLED + 1;
    multiplier = 75.0f / totalLEDs;
    // color wheel has 90 colors between green to yellow to red
    volumeMeterDivider = 90 / totalLEDs;
    debugln("[VolumeCell] init()- start/end, color:   "+String(startLED)+"/"+String(endLED)+ " , "+String(color) + ", "+ String(volumeMeterDivider));
}

void VolumeCell::notify(EventArgs args) {}
void VolumeCell::setPercent(float percent) {
  // debugln("[VolumeCell] setPercent() - percent: " + String(percent));
  if (percent < 5.0f) {
    nextHeight = 0;
  } else if (percent < 10.0f) {
    nextHeight = 1;
  } else {
    nextHeight = (percent > 75.0f) ? totalLEDs : (uint8_t)(percent / multiplier); // convert to 0-255
    if (nextHeight > totalLEDs) {
      nextHeight = totalLEDs;
    }
  }

}

bool VolumeCell::work() {
     if ((millis() - lastUpdate) > interval) {
        lastUpdate = millis();
        if (nextHeight != currentHeight) {
          currentHeight = nextHeight;
          draw();
          return true;
        }
   }
   return false;
}

void VolumeCell::draw() {
    // Draw the current height of the volume cell
    // debug("VolumeCell: draw() - currentHeight: " + String(currentHeight)+"\n");
    for (uint8_t i = startLED; i < endLED + 1; i++) {
        if (i < startLED+currentHeight) {
            _packLeds.setPixelColor(i, Wheel(90-((i-startLED)*volumeMeterDivider)));
            // if (i < uint8_t(startLED+volumeMeterDivider)) {
            //   // green
            //   _packLeds.setPixelColor(i, Wheel(85));
            // } else if (i < uint8_t(startLED+(volumeMeterDivider * 2))) {
            //   // yellow
            //   _packLeds.setPixelColor(i, Wheel(42));
            // } else {
            //   // red
            //   _packLeds.setPixelColor(i, Wheel(255));
            // }
        } else {
            _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
        }
    }
}

void VolumeCell::packOff() {
    activePattern = PC_OFF;
}

void VolumeCell::boot() {
    activePattern = PC_BOOT;
}

void VolumeCell::setIsFiring(bool IsFire) {}

void VolumeCell::setOverheating(int stage) {}

void VolumeCell::idle(bool force = false) {
    activePattern = PC_IDLE;
}

void VolumeCell::powerDown() {
    activePattern = PC_POWERDOWN;
}


void VolumeCell::vent() {
    activePattern = PC_VENT;
}

// clear all the powercell leds
void VolumeCell::clear()
{
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
    }
}
