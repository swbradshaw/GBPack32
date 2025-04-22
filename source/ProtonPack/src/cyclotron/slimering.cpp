#include "slimering.h"
#include "../common.h"


void SlimeRing::idleUpdate()  {
    for (uint8_t i = 0; i < totalLEDs; i++) {
        if (i == index) {
            if (ledArray[index] == 1) {
                _packLeds.setPixelColor(startLED+indexArray, DimColor(_packLeds.getPixelColor(startLED+indexArray)));
                _packLeds.setPixelColor(startLED+indexArray, DimColor(_packLeds.getPixelColor(startLED+indexArray)));
            }
        } else {
            if (ledArray[i] == 1) {
                if (i < totalLEDs) {
                    if (indexArray != i) {
                        uint16_t  previous = startLED+indexArray-1;
                        if (previous < startLED) {
                            previous = endLED;
                        }
                        _packLeds.setPixelColor(previous, color);
                    }
                }
            }
        }
    }
    increment();
}
