#include "WandEngine.h"

// Handles all of the LEDs in the Wand
/*
  0 - Slow blo
  1-5 WAND bar
  6 - Wand indiactor
  7 - Vent
  8 - Firing LED
  9-18 - 10 Barrel LEDs
*/
// FIXME - move Wand LED logic to external class - make it part of the themeing

// constructor for WandEngine
    WandEngine::WandEngine(Adafruit_NeoPixel &strip) :
        _wandLeds(strip),
        _slowBlowIndicator(strip, WAND_LED_SLOWBLOW) ,
        _ventIndicator(strip, WAND_LED_1ST_VENT),
        _timedFiringIndicator(strip, WAND_LED_FIRING),
        _wandOnIndicator(strip, WAND_LED_WAND_ON) {


        // init wand barrel leds
        _numBarrelLeds = WAND_LED_TIP_LAST - WAND_LED_TIP_1ST + 1;
        _barrelLedState = new uint8_t *[_numBarrelLeds];
        for (int i = 0; i < _numBarrelLeds; i++)
        {
            _barrelLedState[i] = new uint8_t[3];
        }
        _numBarLeds = WAND_LED_LAST_BAR - WAND_LED_1ST_BAR + 1;
        _barLedState = new uint8_t *[_numBarLeds];
        for (int i = 0; i < _numBarLeds; i++)
        {
            _barLedState[i] = new uint8_t[3];
        }

        _prevUpdate = 0;
    }

    void WandEngine::init() {
        debugln("WandEngine init");
        _wandLeds.begin();
        _wandLeds.setBrightness(255);
        _wandLeds.clear();

        _slowBlowIndicator.begin();
        _ventIndicator.begin();
        _timedFiringIndicator.begin();
        _wandOnIndicator.begin();

        debug("Wand barrel LEDs:");
        debugln(_numBarrelLeds);
        debug("Wand bar LEDs:");
        debugln(_numBarLeds);

        _wandLeds.show();

    }

    void WandEngine::notify(EventArgs args){
        if (args.eventName.startsWith("EVENT_PACK_")) {
            handleEvent(args);
        }
        if (args.eventName.startsWith("EVENT_WAND_")) {
            handleEvent(args);
        }
        if (args.eventName.equals("EVENT_THEME_CHANGE"))  {
          int themeMode = args.eventDetail1.toInt();
          changeTheme(themeMode);
      }
    }

    void WandEngine::work() {
         if ((millis() - _lastUpdate) > _interval) // Time to update?
        {
            _lastUpdate = millis();
            // do any reactions of immediate state changes here
            if ((!packOn) && (!wandOn)) {
                // wand is off and pack is off

                // is pack booting?
                if (packBooting) {
                    barBooting(WAND_BAR_PACK_BOOT_INTERVAL, WAND_BAR_PACK_BOOT_INTERVAL, false);
                    _slowBlowIndicator.update(CRGB(0).Red, INDICATOR_MEDIUM_FLASH);
                    _wandLeds.show();
                }
                // is wand booting?
                if (wandBooting) {
                    barWandOn(WAND_BAR_IDLE_INTERVAL);
                    _wandLeds.show();
                }
            } else if ((!packOn) && (wandOn)) {
                // wand is on and pack is off
                if (wandShuttingDown) {
                    barShutdown(WAND_BAR_SHUTDOWN_INTERVAL, false);
                } else if (packBooting) {
                    barBooting(WAND_BAR_PACK_BOOT_INTERVAL, WAND_BAR_PACK_BOOT_INTERVAL, false);
                    _slowBlowIndicator.update(CRGB(0).Red, INDICATOR_MEDIUM_FLASH);
                } else {
                    barWandOn(160);
                }
                _wandLeds.show();

            } else if ((packOn) && (!wandOn)) {
                // wand is off and pack is on
                if (!packShuttingDown) {
                    barWandOn(WAND_BAR_IDLE_INTERVAL);
                } else {
                    barShutdown(WAND_BAR_SHUTDOWN_INTERVAL, false);
                }
                _wandLeds.show();
            } else if ((packOn) && (wandOn)) {
                // wand is on and pack is on

                if (wandFiring) {
                    barFiring(WAND_BAR_FIRE_INTERVAL);
                    handleFireStream(WAND_BARREL_FIRE_INTERVAL);
                    updateWandBarrel();
                    _timedFiringIndicator.update(CRGB(0).White, INDICATOR_MEDIUM_FLASH);
                } else if (wandFinishedFiring) {
                    // the wand just finished wiring - we animate the barrel leds shutting down
                    handlePostFireStream(WAND_BARREL_POST_FIRE_INTERVAL);
                    barWandFireIdle(WAND_BAR_FIRE_READY_INTERVAL);
                    updateWandBarrel();
                } else {
                    // animate wand ready to fire
                    barWandFireIdle(WAND_BAR_FIRE_READY_INTERVAL);
                }
                _wandLeds.show();
            }
        }
    }

    void WandEngine::handleEvent(EventArgs args) {
        // handle events
        // we update our various states and initialize the LEDS based on a state change
        debugln("WandEngine handleEvent: " + args.eventName);

        if (args.eventName == EVENT_PACK_BOOTING) {
            packBooting = true;
            barBooting(50, 50, true);
        }
        if (args.eventName == EVENT_PACK_IDLE) {
            packBooting = false;
            packOn = true;
            _slowBlowIndicator.setColor(CRGB(0).Red);
        }
        if (args.eventName == EVENT_PACK_SHUTTING_DOWN) {
            packShuttingDown = true;
            barShutdown(250, true);
        }
        if (args.eventName == EVENT_PACK_OFF) {
            packShuttingDown = false;
            packOn = false;
            _slowBlowIndicator.clear();
            _ventIndicator.clear();
            if (!wandOn) {
                // clear all the LEDS.
                _wandLeds.clear();
                _wandLeds.show();
            }
        }

        if (args.eventName == EVENT_WAND_BOOTING) {
            wandBooting = true;
            _wandOnIndicator.setColor(CRGB(0).Orange);
            barBooting(50, 50, true);
        }
        if (args.eventName == EVENT_WAND_IDLE) {
            wandBooting = false;
            wandOn = true;
            _ventIndicator.setColor(wandVentOn  ? CRGB(0).White : CRGB(0).Black);
        }
        if (args.eventName == EVENT_WAND_SHUTTING_DOWN) {
            wandShuttingDown = true;
            if (!packOn) {
                barShutdown(250, true);
            }
        }
        if (args.eventName == EVENT_WAND_OFF) {
            wandShuttingDown = false;
            wandOn = false;
            _wandOnIndicator.clear();
            _ventIndicator.clear();
             _runningLedTracker = 0;
             if (!packOn) {
                // clear all the LEDS.
                _wandLeds.clear();
            }
            _wandLeds.show();
        }

        // VENT BUTTON SWITCH
        if (args.eventName ==  EVENT_VENT_WAND_ON) {
            wandVentOn = true;
            if (wandOn) {
                _ventIndicator.setColor(CRGB(0).White);
                _wandLeds.show();
            }
        }
        if (args.eventName ==  EVENT_VENT_WAND_OFF) {
            wandVentOn = false;
            if (wandOn) {
                _ventIndicator.clear();
                _wandLeds.show();
            }
        }
        if (args.eventName == EVENT_WAND_FIRING_START) {
            wandFiring = true;
            initFireColors();
            updateWandBarrel();
        }
        if (args.eventName == EVENT_WAND_FIRING_STOP) {
            wandFiring = false;
            wandFinishedFiring = true;
            initPostWandFire();
            _timedFiringIndicator.update(CRGB(0).Black, 0);
        }
    }



  void WandEngine::changeTheme(int themeMode) {
    debugln("changeTheme in WandEngine");


    c_temp_effect = C_BLUE;
    c_temp_start = C_RED;
    // switch based on themeMode
    switch (themeMode) {
      case 6: // Hannukah
        wandBarColor = 0x0000FF;
        break;
      case 7: // St. Pattys
        wandBarColor = 0x00FF00;
        c_temp_effect = C_WHITE;
        c_temp_start =  C_DARK_GREEN;
        break;

      default: // 1984
        wandBarColor = 0xFFA500;
        break;
    }

  }
    /**************** START WAND BARREL FUNTIONS ***********************/
    #pragma region wand barrel functions

    void WandEngine::updateWandBarrel() {
        for (uint8_t i = 0; i < _numBarrelLeds; i++)
        {
            _wandLeds.setPixelColor(WAND_LED_TIP_1ST + i, _barrelLedState[i][0], _barrelLedState[i][1], _barrelLedState[i][2]);
        }
    }

    void WandEngine::setBarrelColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
        _barrelLedState[pixel][0] = red;
        _barrelLedState[pixel][1] = green;
        _barrelLedState[pixel][2] = blue;
    }

    void WandEngine::setBarrelColorAll(uint8_t red, uint8_t green, uint8_t blue) {
        for (uint16_t i = 0; i < _numBarrelLeds; i++)
        {
            _barrelLedState[i][0] = red;
            _barrelLedState[i][1] = green;
            _barrelLedState[i][2] = blue;
        }
    }

    void WandEngine::initFireColors() {

        CRGB color = getHueColour(c_temp_start, 255);
        for(uint8_t i = 0; i < _numBarrelLeds; i++) {
            _barrelLedState[i][0] = color.g;
            _barrelLedState[i][1] = color.r;
            _barrelLedState[i][2] = color.b;
        }
    }
    void WandEngine::initPostWandFire() {

        debugln("initPostWandFire");
        i_barrel_light = _numBarrelLeds;
    }
    void WandEngine::handlePostFireStream(uint8_t updateInterval) {
        // basically set the current barrel light to off, then decrement the barrel lights back to start
        if (millis() - _prevUpdate > updateInterval)
        {
            _prevUpdate += updateInterval;
            if (i_barrel_light < _numBarrelLeds) {
                _barrelLedState[i_barrel_light][0] = 0;
                _barrelLedState[i_barrel_light][1] = 0;
                _barrelLedState[i_barrel_light][2] = 0;
            }
            if (i_barrel_light == 0)  {
                // our work is done
                wandFinishedFiring = false;
            } else {
                // keep going down the barrel
                i_barrel_light--;
            }
        }
    }
    void WandEngine::handleFireStream(uint8_t updateInterval) {
        CRGB c_colour = getHueColour(c_temp_effect, 255);
        if (millis() - _prevUpdate > updateInterval)
        {
            initFireColors();
            _prevUpdate += updateInterval;
            if (i_barrel_light == _numBarrelLeds)  {
                i_barrel_light = 0;
            } else if (i_barrel_light < _numBarrelLeds) {
                _barrelLedState[i_barrel_light][0] = c_colour.g;
                _barrelLedState[i_barrel_light][1] = c_colour.r;
                _barrelLedState[i_barrel_light][2] = c_colour.b;


                uint8_t i_t_rand = random(0, _numBarrelLeds / 4);
                for(uint8_t i = i_barrel_light + 1; i < i_barrel_light + i_t_rand; i++) {
                    if (i < _numBarrelLeds) {
                        _barrelLedState[i][0] = c_colour.g;
                        _barrelLedState[i][1] = c_colour.r;
                        _barrelLedState[i][2] = c_colour.b;
                    }
                }
            }
            i_barrel_light++;
        }
    }

    #pragma endregion
    /**************** END WAND BARREL FUNTIONS ***********************/



    /**************** START WAND BAR FUNTIONS ***********************/
   #pragma region wand bar functions
    void WandEngine::barWandOn(uint8_t idleTime) {
        // animate bar when wand is turning on
        if (millis() - _prevBarTime >= idleTime) {
            _prevBarTime = millis();
            for (int8_t i = 0; i < _numBarLeds; i++) {
                // All segements equal and below running led tracker will be ON
                if (_runningLedTracker <= 0) {
                    setBarColor(i, 0, 0, 0);
                } else {
                    if (i <= _runningLedTracker - 1) {
                        setBarPixelOn(i, CRGB(wandBarColor));
                    } else {
                        setBarColor(i, 0, 0, 0);
                    }
                }
            }
            if (_reverseSeqTracker == false) {
                _runningLedTracker++;
                if (_runningLedTracker > _numBarLeds) {
                    _runningLedTracker = _numBarLeds;
                    _reverseSeqTracker = true;
                }
            } else {
                if (_runningLedTracker > 0) {
                    _runningLedTracker--;
                }
                if (_runningLedTracker == 0) {
                    _reverseSeqTracker = false;
                }
            }
            updateWandBar();
        }
    }

    void WandEngine::barWandFireIdle(uint8_t idleTime) {
        // animation when wand is ready to fire
        if (millis() - _prevBarTime >= idleTime)
        {
            _prevBarTime = millis();
            for (int8_t i = 0; i < _numBarLeds; i++) {
                if (i == _runningLedTracker - 1) {
                    setBarPixelOn(i, CRGB(wandBarColor));
                } else {
                    setBarColor(i, 0, 0, 0);
                }
            }
            if (_reverseSeqTracker == false) {
                _runningLedTracker++;
                if (_runningLedTracker >= _numBarLeds) {
                    _runningLedTracker = _numBarLeds;
                    _reverseSeqTracker = true;
                }
            }
            else {
                _runningLedTracker--;
                if (_runningLedTracker <= 1) {
                    _runningLedTracker = 1;
                    _reverseSeqTracker = false;
                }
            }
            updateWandBar();
        }
    }

    void WandEngine::barFiring(uint8_t firingSpeedInterval) {
        if (millis() - _prevBarTime >= firingSpeedInterval) {
            _prevBarTime = millis();
            for (int8_t i = 0; i < _numBarLeds; i++) {
                 if ((i == _fireSeqTracker) || (i == (_numBarLeds - 1) - _fireSeqTracker)) {
                    setBarPixelOn(i, CRGB(wandBarColor));
                } else {
                    setBarColor(i, 0, 0, 0);
                }
            }
             if (_reverseSeqTracker == false) {
                _fireSeqTracker++;
                if (_fireSeqTracker >= ((_numBarLeds / 2))) {
                    _fireSeqTracker = ((_numBarLeds / 2));
                    _reverseSeqTracker = true;
                }
            } else {
                if (_fireSeqTracker > 0) {
                    _fireSeqTracker--;
                }
                if (_fireSeqTracker <= 0) {
                    _reverseSeqTracker = false;
                }
            }

            updateWandBar();
        }
    }

    void WandEngine::barBooting(uint8_t bootSp, uint8_t idleTime, bool init) {
        static bool wandBarComplete = false;
        if (init) {
            _runningLedTracker = _numBarLeds;
            _bootState = false;
            wandBarComplete = false;
        }
        if (!_bootState) {
            if (millis() - _prevBarTime >= bootSp) {
                _prevBarTime = millis();
                if (!wandBarComplete) {
                    for (int8_t i = _numBarLeds - 1; i >= 0; i--) {
                        if (i >= _runningLedTracker) {
                            setBarPixelOn(i, CRGB(wandBarColor));
                        } else {
                            setBarColor(i, 0, 0, 0);
                        }
                    }
                    if (_runningLedTracker >= 0) {
                        _runningLedTracker--;
                    }
                    if (_runningLedTracker < 0) {
                        wandBarComplete = true;
                        _runningLedTracker = _numBarLeds;
                    }
                }
                else {
                    for (int8_t i = _numBarLeds - 1; i >= 0; i--) {
                        if (i >= _runningLedTracker) {
                            setBarColor(i, 0, 0, 0);
                        } else {
                            setBarPixelOn(i, CRGB(wandBarColor));
                        }
                    }
                    if (_runningLedTracker >= 0) {
                        _runningLedTracker--;
                    }
                    if (_runningLedTracker < 0) {
                        _bootState = true;
                        _runningLedTracker = 0;
                    }
                }
                updateWandBar();
            }

        } else {
            barWandOn(idleTime);
        }
    }


    void WandEngine::barShutdown(uint8_t shutdownSp, bool init) {
        if (init) {
            _runningLedTracker = _numBarLeds;
            _bootState = true;
        }
         if (_bootState) {
            if (millis() - _prevBarTime >= shutdownSp) {
                _prevBarTime = millis();
                 for (int8_t i = _numBarLeds - 1; i >= 0; i--) {
                    if (i >= _runningLedTracker) {
                        setBarColor(i, 0, 0, 0);
                    } else {
                        setBarPixelOn(i, CRGB(wandBarColor));
                    }
                }
                if (_runningLedTracker >= 0) {
                    _runningLedTracker--;
                }
                if (_runningLedTracker < 0) {
                    _bootState = false;
                    _runningLedTracker = 0;
                }
                updateWandBar();
            }
        } else {
            _wandLeds.clear();
        }
    }

    void WandEngine::updateWandBar() {
        for (uint8_t i = 0; i < _numBarLeds; i++) {
            _wandLeds.setPixelColor(WAND_LED_1ST_BAR + i, _barLedState[i][0], _barLedState[i][1], _barLedState[i][2]);
        }
    }

    void WandEngine::setBarPixelOn(uint16_t pixel, CRGB color) {
        _barLedState[pixel][0] = color.r;
        _barLedState[pixel][1] = color.g;
        _barLedState[pixel][2] = color.b;
    }
    void WandEngine::setBarColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
        _barLedState[pixel][0] = red;
        _barLedState[pixel][1] = green;
        _barLedState[pixel][2] = blue;
    }

    void WandEngine::setBarColorAll(uint8_t red, uint8_t green, uint8_t blue)
    {
        for (uint16_t i = 0; i < _numBarrelLeds; i++)
        {
            _barLedState[i][0] = red;
            _barLedState[i][1] = green;
            _barLedState[i][2] = blue;
        }
    }
    #pragma endregion
    /**************** END WAND BARREL FUNTIONS ***********************/

