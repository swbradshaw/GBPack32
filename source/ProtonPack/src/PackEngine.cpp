#include "PackEngine.h"

// constructor for PackEngine
PackEngine::PackEngine(Adafruit_NeoPixel &strip) : _packLeds(strip) {
  numVentLeds = PACK_LED_VENT_LAST - PACK_LED_VENT_1ST + 1;
}

void PackEngine::init() {
  debugln("PackEngine init");
  _packLeds.begin();
  _packLeds.setBrightness(255);
  _packLeds.clear();
  debug("Vent LEDs:");
  debugln(numVentLeds);

  _packLeds.show();
  delayTimer.start(150);
}
void PackEngine::changeTheme(int themeMode) {
  debugln("PackEngine: Changed Theme: " + themeMode);
  delete powercell;
  delete cyclotron;

  /* Some nasty hard-coding of positions of my LED ring light */
  uint8_t topLeft = 8;
  uint8_t topRight = 29;
  uint8_t bottomLeft = topLeft + 1;
  uint8_t bottomRight = topRight - 1;
  smokeColor = 150;
  sendVolumeEvents = false;
  ventSwitchTogglesVent = false;
  // switch based on themeMode
  // FIXME - move this into some kind of theme management system
  switch (themeMode) {
    case 0:  // 1984
      powercell = new EightyFour(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(170));
      powercell->init();
      cyclotron = new LedRing(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(255));
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,255,0,0,0,0");
      break;
    case 1:  // AFTERLIFE
      powercell = new EightyFour(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(170));
      powercell->init();
      /* March 2025 - Modified afterife ring to take in a static list of LED positions to
        cycle through This is defind as ledRingVisibleLeds in the header file. This allows us
        to bypass lighting up LEDs that arent visible and makes the animation faster.
      */

      cyclotron = new AfterlifeRing(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(255),
                                    ledRingVisibleLeds, 16);
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,255,0,0,0,0");
      break;
    case 2:  // SLIME
      powercell = new EightyFour(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(170));
      powercell->init();

      cyclotron = new SlimeRing(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(85));
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,0,0,255,0,0");
      break;
    case 3:  // MESON
      powercell = new EightyFour(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(170));
      powercell->init();

      cyclotron = new LedRing(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(42));
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,129,0,126,0,0");
      break;
    case 4:  // CHRISTMAS
      powercell = new EightyFour(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(170));
      powercell->init();

      cyclotron = new ChristmasRing(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(85));
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,0,0,255,0,0");
      break;
    case 5:  // KITT / knight rider
      powercell = new KittPowercell(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(255), 3);
      powercell->init();
      cyclotron = new DoubleSway(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(255), 5, topLeft,
                                 topRight, bottomLeft, bottomRight);
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,255,0,0,0,0");
      ventSwitchTogglesVent = true;
      break;
    case 6:  // Hannukah
      powercell = new SnowPowercell(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(170));
      powercell->init();
      cyclotron = new Dredel(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(170));
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,0,0,0,0,255");
      break;
    case 7:  // St. Patrick
      powercell = new EightyFour(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(85));
      powercell->init();
      cyclotron = new Alternate(_packLeds, topLeftLeds, topRightLeds, bottomLeftLeds, bottomRightLeds, Wheel(85));
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,0,0,255,0,0");
      smokeColor = 85;
      break;
    case 8:  // Baseball
      powercell = new VolumeCell(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(85));
      // powercell = new EightyFour(_packLeds, PACK_LED_POWERCELL_1ST,
      // PACK_LED_POWERCELL_LAST, Wheel(85));
      powercell->init();
      cyclotron = new Volumetron(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(170));
      cyclotron->init();
      sendVolumeEvents = true;
      ventSwitchTogglesVent = true;
      break;

    default:  // 1984
      powercell = new EightyFour(_packLeds, PACK_LED_POWERCELL_1ST, PACK_LED_POWERCELL_LAST, Wheel(170));
      powercell->init();
      cyclotron = new LedRing(_packLeds, PACK_LED_CYCLOTRON_1ST, PACK_LED_CYCLOTRON_LAST, Wheel(255));
      cyclotron->init();
      cyclotron->setCustomValue(FADE_SETTING, "0,255,0,0,0,200");
      break;
  }

  // Tell audio engine if this theme wants volume level events
  EventArgs volumeEvents;
  volumeEvents.eventName = EVENT_AUDIO_SEND_VOLUME_EVENTS;
  volumeEvents.eventDetail1 = sendVolumeEvents ? "1" : "0";
  Subject::notify(volumeEvents);

  if (packOn) {
    // user changed themes while pack was on, attempt to force idle state
    cyclotron->idle();
    powercell->idle(true);
  }
}

void PackEngine::notify(EventArgs args) {
  if (args.eventName.startsWith("EVENT_PACK_")) {
    handleEvent(args);
  }
  if (args.eventName.startsWith("EVENT_WAND_")) {
    handleEvent(args);
  }
  if (args.eventName.startsWith("EVENT_AUDIO")) {
    handleAudioEvent(args);
  }
  if (args.eventName.startsWith("EVENT_OVERHEAT")) {
    handleOverheatEvent(args);
  }
  if (args.eventName.equals("EVENT_THEME_CHANGE")) {
    changeTheme(args.eventDetail1.toInt());
  }
  if (args.eventName.startsWith("EVENT_SMOKE")) {
    handleEvent(args);
  }
}

void PackEngine::handleAudioEvent(EventArgs args) {
  // debugln("handleAudioEvent in PackEngine: " + args.eventName);
  if (args.eventName == EVENT_AUDIO_STOP_SOUND_CLIP) {
    powercell->notify(args);
  }
  if (args.eventName == EVENT_AUDIO_STOP_MOVIE_QUOTE) {
    powercell->notify(args);
  }
  if (args.eventName == EVENT_AUDIO_PLAY_SOUND_CLIP) {
    powercell->notify(args);
  }
  if (args.eventName == EVENT_AUDIO_PLAY_MOVIE_QUOTE) {
    powercell->notify(args);
  }
  if (args.eventName == EVENT_AUDIO_VOLUME && sendVolumeEvents) {
    float volumePercent = args.eventDetail1.toFloat();
    powercell->setPercent(volumePercent);
    cyclotron->setPercent(volumePercent);
  }
}

void PackEngine::handleOverheatEvent(EventArgs args) {
  if (args.eventName == EVENT_OVERHEAT) {
    int stage = args.eventDetail1.toInt();
    // Tell our powercell and cyclotron about the overheating stage
    powercell->setOverheating(stage);
    cyclotron->setOverheating(stage);
    // if we've reached stage 5 (vent), turn on the vent light on the cycltotron
    if (stage == 5) {
      enableVentLights(true);
    }
  }
  if (args.eventName == EVENT_OVERHEAT_COMPLETE) {
    // sent from audio engine
    enableVentLights(false);
    wandFiring = false;
    powercell->setIsFiring(false);
    cyclotron->setIsFiring(false);
    lastOverheatStage = 0;
    powercell->idle(true);
    cyclotron->idle();
  }
}

void PackEngine::handleOverheat() {
  // calculate the stage of overheating
  // 1 -faster lights, 2 - even faster ligths, 3 = faster lights,  stream w/beeping, 4 -
  // overheated; beeping only, 5 - start venting
  long timeSinceStart = millis() - wandFiringStart;
  uint32_t stage = getOverheatStage(timeSinceStart);
  if (stage != lastOverheatStage) {
    lastOverheatStage = stage;
    debugln("New Overheat stage: " + String(lastOverheatStage));
    sendOverheatEvent();
  }
}

void PackEngine::sendOverheatEvent() {
  EventArgs overheatEvent;
  overheatEvent.eventName = EVENT_OVERHEAT;
  overheatEvent.eventDetail1 = String(lastOverheatStage);
  Subject::notify(overheatEvent);
  handleOverheatEvent(overheatEvent);
}

void PackEngine::work() {
  // tell our cyclotron and powercell to do their work
  // if they return true, we need to update the leds
  bool powercellWork = powercell->work();
  bool cyclotronWork = cyclotron->work();

  if (powercellWork || cyclotronWork) {
    _packLeds.show();
  }

  // check for overheating, which is based off how long wand has been firing
  if (delayTimer.justFinished()) {
    if ((wandFiring) && (!wandVentOn)) {
      // for how long? notify wand and pack that its overheating?
      if (millis() - wandFiringStart > getOverheatTime()) {
        // overheat
        handleOverheat();
      }
    }
    delayTimer.restart();
  }
}

void PackEngine::enableVentLights(bool OnorOff) {
  uint32_t color = OnorOff ? Wheel(smokeColor) : 0;
  for (uint8_t i = 0; i < numVentLeds; i++) {
    _packLeds.setPixelColor(PACK_LED_VENT_1ST + i, color);
  }
  _packLeds.show();
}

/*
  Dispatch events to cycltron and powercell
  as well as handle n-filter lights
*/
void PackEngine::handleEvent(EventArgs args) {
  debugln("PackEngine handleEvent: " + args.eventName);

  if (args.eventName == EVENT_SMOKE_ON) {
    enableVentLights(true);
  }
  if (args.eventName == EVENT_SMOKE_OFF) {
    enableVentLights(false);
  }

  if (args.eventName == EVENT_PACK_BOOTING) {
    packBooting = true;
    cyclotron->boot();
    powercell->boot();
  }
  if (args.eventName == EVENT_PACK_IDLE) {
    packBooting = false;
    packOn = true;
    cyclotron->idle();
    powercell->idle(false);
  }
  if (args.eventName == EVENT_VENT_WAND_ON) {
    wandVentOn = true;
    if (ventSwitchTogglesVent) {
      // toggle relay
      EventArgs smokeEvent;
      smokeEvent.eventName = EVENT_SMOKE_TURN_ON;
      smokeEvent.eventDetail1 = "30000";  // 6 seconds
      Subject::notify(smokeEvent);
      enableVentLights(true);
    }
  }
  if (args.eventName == EVENT_VENT_WAND_OFF) {
    wandVentOn = false;
    if (ventSwitchTogglesVent) {
      // toggle relay
      EventArgs smokeEvent;
      smokeEvent.eventName = EVENT_SMOKE_TURN_OFF;
      Subject::notify(smokeEvent);
      enableVentLights(false);
    }
  }
  if (args.eventName == EVENT_WAND_FIRING_START) {
    wandFiring = true;
    wandFiringStart = millis();
    cyclotron->setIsFiring(true);
    powercell->setIsFiring(true);
  }
  if (args.eventName == EVENT_WAND_FIRING_STOP) {
    wandFiring = false;
    cyclotron->setIsFiring(false);
    powercell->setIsFiring(false);
    if (lastOverheatStage < 4) {
      // wand stop firing; notify there is no overheating
      // if there was an overheat going on
      lastOverheatStage = 0;
      sendOverheatEvent();
    }
  }
  if (args.eventName == EVENT_PACK_SHUTTING_DOWN) {
    packShuttingDown = true;
    powercell->powerDown();
    cyclotron->powerDown();
  }
  if (args.eventName == EVENT_PACK_OFF) {
    packOn = false;
    packShuttingDown = false;
    cyclotron->packOff();
    powercell->packOff();
    _packLeds.show();
  }
}

// hardcoded - how long after stream starts before overheat starts?
uint32_t PackEngine::getOverheatTime() { return 4000; }

// hardcoded values for overheat stages
uint8_t PackEngine::getOverheatStage(unsigned long timeSinceStart) {
  if (timeSinceStart < 4000) {
    return 0;
  }
  if (timeSinceStart < 7500) {
    return 1;
  }
  if (timeSinceStart < 9000) {
    return 2;
  }
  if (timeSinceStart < 13000) {
    return 3;
  }
  if (timeSinceStart < 15000) {
    return 4;
  }
  return 5;
}