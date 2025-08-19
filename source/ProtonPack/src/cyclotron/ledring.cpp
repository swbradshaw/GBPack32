#include "ledring.h"
#include "../common.h"

void LedRing::setPercent(float percent) {}

void LedRing::init() {
    totalLEDs = endLED - startLED + 1;
    debugln("LedRing: init "+String(startLED)+" "+String(endLED));
    activePattern = OFF;
}

bool LedRing::work() {
     if ((millis() - lastUpdate) > interval) {
        lastUpdate = millis();
        switch (activePattern) {
            case BOOT:
                bootUpdate();
                break;
            case IDLE:
                idleUpdate();
                break;
            case POWERDOWN:
                powerDownUpdate();
                break;
            case OVERHEATED:
                overheatUpdate();
                break;
            case VENT:
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

void LedRing::setOverheating(int stage) {
  overHeatStage = stage;
  switch(overHeatStage) {
    case 0:
      setInterval();
      break;
    case 1:
      interval = 30;
      break;
    case 2:
      interval = 20;
      break;
    case 3:
      interval = 20;
      break;
    case 4:
      overheated();
      break;
    case 5:
      vent();
      break;
  }
}

void LedRing::setCustomValue(int name, String value){
    customValues[name] = value;
}

void LedRing::packOff() {
    activePattern = OFF;
}

void LedRing::boot() {
    activePattern = BOOT;
    indexArray = 0;
    index = 0;
    fadeStep = 0;
    setInterval();
    totalSteps = 20;
}

void LedRing::bootUpdate() {
    if (customValues[FADE_SETTING]) {
        // fade custom values
        String fadeSettings = customValues[FADE_SETTING];

        byte redStart = get_token(fadeSettings, 0).toInt();
        byte redEnd = get_token(fadeSettings, 1).toInt();
        byte greenStart = get_token(fadeSettings, 2).toInt();
        byte greenEnd = get_token(fadeSettings, 3).toInt();
        byte blueStart = get_token(fadeSettings, 4).toInt();
        byte blueEnd = get_token(fadeSettings, 5).toInt();
        // split fadeSettings by comma
        fade(redStart, redEnd, greenStart, greenEnd, blueStart, blueEnd, totalSteps, startLED, endLED);
    } else {
        // default fade
        fade(0, 255, 0, 0, 0, 0, CYCLOTRON_BOOT_FADE_TIMER, startLED, endLED);
    }
}

void LedRing::idle() {
  activePattern = IDLE;
  totalSteps = totalLEDs;
  setInterval();
  index = 0;
  fadeStep = 0;
  fadeComplete = false;
}

void LedRing::idleUpdate() {
    for (uint8_t i = 0; i < totalLEDs; i++) {
        if (i == index) {
            if (ledArray[index] == 1) {
                _packLeds.setPixelColor(startLED+indexArray, color);
            }
        } else {
            if (i < totalLEDs) {
                _packLeds.setPixelColor(startLED+i, DimColor(_packLeds.getPixelColor(startLED+i)));
            }
        }
      }
      increment();
}

void LedRing::setIsFiring(bool IsFire) {
    isFiring = IsFire;
}

void LedRing::setInterval() {
    // todo - this should be passed in like the colors
    if (activePattern == BOOT) {
        interval = CYCLOTRON_BOOT_INTERVAL;
    } else {
        interval = CYCLOTRON_IDLE_INTERVAL;
    }
}

void LedRing::increment() {
  if (activePattern == IDLE) {
      if (ledArray[index] == 1) {
        indexArray++;
      }
      index++;
      if (index >= (totalSteps))
      {
        index = 0;
        indexArray = 0;
      }
    }
}

void LedRing::powerDown() {
    activePattern = POWERDOWN;
    totalSteps = totalLEDs;
    setInterval();
    index = 0;
}

void LedRing::powerDownUpdate() {
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, DimColor(_packLeds.getPixelColor(i)));
    }
}

void LedRing::overheated() {
    activePattern = OVERHEATED;
    interval = 180;
    index = 0;
}

void LedRing::overheatUpdate() {
  // blink the ring
  int blinkColor = (index == 0) ? 0: color;
  for (uint8_t i = startLED; i < endLED+1; i++) {
      _packLeds.setPixelColor(i, blinkColor);
  }
  index = (index == 0) ? 1 : 0;
}

void LedRing::vent() {
    activePattern = VENT;
    totalSteps = totalLEDs;
    interval = 400;
    index = 0;
    // turn all colors on
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, color);
    }
}
void LedRing::ventUpdate() {
  // fade out - which is the same as power off
  powerDownUpdate();
}

void LedRing::clear()
{
    for (uint8_t i = startLED; i < endLED+1; i++) {
        _packLeds.setPixelColor(i, _packLeds.Color(0, 0, 0));
    }
}
/*
    This is a fading function that is used to help with the cyclotron fading and power up.

    Its sligntly modified to help fade only selected LED's that are called.
*/
void LedRing::fade(byte redStartValue, byte redEndValue, byte greenStartValue, byte greenEndValue, byte blueStartValue, byte blueEndValue, int totalSteps, int startLED, int endLED)
{
  static float redIncrement, greenIncrement, blueIncrement;
  static float red, green, blue;
  static bool fadeUp = false;

  if (fadeStep == 0) // first step is to initialise the initial colour and increments
  {
    red = redStartValue;
    green = greenStartValue;
    blue = blueStartValue;
    fadeUp = false;

    redIncrement = (float)(redEndValue - redStartValue) / (float)totalSteps;
    greenIncrement = (float)(greenEndValue - greenStartValue) / (float)totalSteps;
    blueIncrement = (float)(blueEndValue - blueStartValue) / (float)totalSteps;
    fadeStep = 1; // next time the function is called start the fade

  }
  else  // all other steps make a new colour and display it
  {
    // make new colour
    red += redIncrement;
    green +=  greenIncrement;
    blue += blueIncrement;

    // set up the pixel buffer
    for (byte i = startLED; i <= endLED; i++)
    {
      _packLeds.setPixelColor(i, _packLeds.Color((byte)red, (byte)green, (byte)blue));
    }

    // go on to next step
    fadeStep += 1;

    // finished fade
    if (fadeStep >= totalSteps)
    {
      fadeComplete = true;
      if ((byte)red > 0 && (byte)red < 50)
      {
        for (byte i = startLED; i <= endLED; i++)
        {
          _packLeds.setPixelColor(i, 0);
        }
      }
      if ((byte)green > 0 && (byte)green < 50)
      {
        for (byte i = startLED; i <= endLED; i++)
        {
          _packLeds.setPixelColor(i, 0);
        }
      }
      if ((byte)blue > 0 && (byte)blue < 50)
      {
        for (byte i = startLED; i <= endLED; i++)
        {
          _packLeds.setPixelColor(i, 0);
        }
      }
      if (fadeUp) // finished fade up and back
      {
        fadeStep = 0;

        return; // so next call recalabrates the increments
      }

      // now fade back
      fadeUp = true;
      redIncrement = -redIncrement;
      greenIncrement = -greenIncrement;
      blueIncrement = -blueIncrement;
      fadeStep = 1; // don't calculate the increments again but start at first change
    }
  }
}
