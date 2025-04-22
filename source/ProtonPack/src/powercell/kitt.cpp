#include "kitt.h"
#include "../common.h"

void KittPowercell::init() {
    totalLEDs = endLED - startLED + 1;
    startColor = color;
    centerIndex = (endLED + startLED) / 2; // fixme - wont work on even number of leds
    debugln("[KittPowercell] init()- start/end, color:   "+String(startLED)+"/"+String(endLED)+ " , "+String(color));
    debugln("centerIndex: "+String(centerIndex));
}

bool KittPowercell::work() {
     if ((millis() - lastUpdate) > interval) {
        lastUpdate = millis();
        switch (activePattern) {
            case PC_BOOT:
                bootUpdate();
                break;
            case PC_IDLE:
                if (!speaking) {
                    idleUpdate();
                } else {
                    speechUpdate();
                }
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

void KittPowercell::notify(EventArgs args) {
    debugln("[KittPowercell] notify()- event:   "+args.eventName);

    if (args.eventName == EVENT_AUDIO_STOP_SOUND_CLIP) {
        color = startColor;
        speaking = false;
        clear();
        setInterval();
    }
    if (args.eventName == EVENT_AUDIO_PLAY_SOUND_CLIP) {
        color = speechColor;
        speaking = true;
        clear();
        setInterval();

    }
}

void KittPowercell::packOff() {
    activePattern = PC_OFF;
}

void KittPowercell::boot() {
    // initialize the LED count
    totalLEDs = endLED - startLED + 1;
    // boot up the powercell
    activePattern = PC_BOOT;
    segmentIndex = 0;
    totalSteps = endLED;
    index = endLED;
    direction = PC_REVERSE;
    color = startColor;
    speaking = false;
    setInterval();
}

void KittPowercell::bootUpdate() {
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

void KittPowercell::setIsFiring(bool IsFire) {

}


void KittPowercell::idle(bool force) {
    activePattern = PC_IDLE;
    direction = PC_FORWARD;
    clear();
    setInterval();

    totalSteps = endLED;
    index = startLED;
}

void KittPowercell::idleUpdate() {
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


void KittPowercell::speechUpdate() {
    for (int8_t i = startLED; i < endLED+1; i++) {
        if (i == centerIndex) {
            _packLeds.setPixelColor(i, speechColor);
        } else {
            if (abs(i - centerIndex) < speechDistance[speechIndex]) {
                _packLeds.setPixelColor(i, speechColor);
            } else {
                _packLeds.setPixelColor(i, 0);
            }
        }
    }
    incrementSpeech();
}
void KittPowercell::incrementSpeech() {
    speechIndex++;
    if (speechIndex > 9) {
        speechIndex = 0;
    }
}

void KittPowercell::setInterval() {
    if (activePattern == PC_BOOT) {
        interval = 80;
    } else if (speaking) {
        interval = 20;
    } else if (activePattern == PC_POWERDOWN) {
        interval = 200;
    } else {
        interval = 55;
    }
}

void KittPowercell::setOverheating(int stage) {
     // not implemented
}

/*
    Increment the Powercell LED state
*/
void KittPowercell::increment() {
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
            if (segmentIndex == totalLEDs) {
                // boot complete, let state to idle
            } else if (index - segmentIndex < startLED) {
              index = totalSteps - 1;
              segmentIndex++;
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


void KittPowercell::powerDown() {
    activePattern = PC_POWERDOWN;
    setInterval();
    index = startLED;
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, color);
    }
}

void KittPowercell::powerDownUpdate() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, DimColor(_packLeds.getPixelColor(i)));
    }
}

void KittPowercell::vent() {
    activePattern = PC_VENT;
    direction = PC_FORWARD;
    setInterval();
    index = startLED;
}
void KittPowercell::ventUpdate() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, color);
    }
}

// clear all the powercell leds
void KittPowercell::clear()
{
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
    }
}
