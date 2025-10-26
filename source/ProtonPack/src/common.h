#pragma once

#include "Arduino.h"
#include <Preferences.h>

Preferences getPrefs();
void closePrefs();
String get_token(String &from, uint8_t index);

/* DEBUG information */
bool const DEBUG = true;
#define DEBUG 1

// Debug macros
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

// ESP32 GPIO PINS

// BUTTONS
const byte WAND_FIRE_BUTTON = 0;
const byte PACK_BUTTON = 32;
const byte WAND_TOGGLE_BUTTON = 17;
const byte WAND_VENT_TOGGLE_BUTTON = 33;
const byte WAND_SPECIAL_BUTTON = 26;

// ROTARY WAND control
const byte ROTARY_CLK_PIN = 13;
const byte ROTARY_DT_PIN = 35;
const byte ROTARY_PUSH_BUTTON_PIN = 4;

// SD-CARD
const byte SD_CARD_CS_PIN = 5;
// these other SD card pins can't currently be changed - leaving here for reference
const byte SD_CARD_MISO_PIN = 19;
const byte SD_CARD_MOSI_PIN = 23;
const byte SD_CARD_CLK_PIN = 18;

// External Audio DAC
const uint8_t DAC_PIN_DCK = 14;
const uint8_t DAC_PIN_WS = 15;
const uint8_t DAC_PIN_DATA_OUT = 16;

// LED lights
const byte WAND_LED_PIN = 27;
const byte PACK_LED_PIN = 12;

// Smoke relay pin
const byte SMOKE_RELAY_PIN = 25;


const uint8_t WAND_TOTAL_LEDS_NUMBER = 19;
const uint8_t WAND_LED_SLOWBLOW = 0;
const uint8_t WAND_LED_1ST_BAR = 1;
const uint8_t WAND_LED_LAST_BAR = 5;
const uint8_t WAND_LED_FIRING = 6;
const uint8_t WAND_LED_1ST_VENT = 7;
const uint8_t WAND_LED_LAST_VENT = 7;
const uint8_t WAND_LED_WAND_ON = 8;
const uint8_t WAND_LED_TIP_1ST = 9;
const uint8_t WAND_LED_TIP_LAST = 16;


// Cyclotron + PowerCell + Vent LED Count
const uint8_t PACK_TOTAL_LEDS_NUMBER = 5 + 40 + 15;

const uint8_t PACK_LED_VENT_1ST = 0;
const uint8_t PACK_LED_VENT_LAST = 4;

const uint8_t PACK_LED_CYCLOTRON_1ST = PACK_LED_VENT_LAST +1;
const uint8_t PACK_LED_CYCLOTRON_LAST = PACK_LED_CYCLOTRON_1ST + 39;

const uint8_t PACK_LED_POWERCELL_1ST = PACK_LED_CYCLOTRON_LAST + 1;
const uint8_t PACK_LED_POWERCELL_LAST = PACK_LED_POWERCELL_1ST+ 14;

// led timings - the lower the number, the faster the animation


const uint8_t WAND_BARREL_FIRE_INTERVAL = 50; // wand barrel firing
const uint8_t WAND_BARREL_POST_FIRE_INTERVAL = 160; // wand barrel post firing
const uint8_t WAND_BAR_FIRE_READY_INTERVAL = 70; // wand bar when pack and wand are on, ready to fire
const uint8_t WAND_BAR_FIRE_INTERVAL = 80; // wand bar when firing
const uint8_t WAND_BAR_SHUTDOWN_INTERVAL = 250; // wand bar when pack is shuting down
const uint8_t WAND_BAR_PACK_BOOT_INTERVAL = 120; // wand bar when pack is booting up
const uint8_t WAND_BAR_IDLE_INTERVAL = 160; // wand bar when pack is idle

const uint8_t CYCLOTRON_BOOT_FADE_TIMER = 100; // how fast the cyclotron fades on boot
const uint8_t CYCLOTRON_BOOT_INTERVAL = 150;
const uint8_t CYCLOTRON_IDLE_INTERVAL = 95;

const uint8_t POWERCELL_UPDATE_INTERVAL= 75;
const uint8_t POWERCELL_BOOT_INTERVAL= 15;

/* maximum time that the smoke relay can be ON */
const uint32_t SMOKER_MAX_ON_TIME = 30000; // in ms
/* minimum OFF time of the smoke relay */
const uint32_t SMOKER_MIN_OFF_TIME = 2000; // in ms