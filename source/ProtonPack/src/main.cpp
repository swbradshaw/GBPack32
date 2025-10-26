/*
  ESP32 Event-based Ghostbusters Proton Pack/Wand engine
  Written by Scott Bradshaw

*/

#include "Arduino.h"

#include <SD.h>
#include <PinButton.h>
#include "errors.h"
#include "menu/menu.h"
#include "bluetooth/bluetooth.h"
#include "main.h"
#include "common.h"
#include "input/ButtonReader.h"
#include "audio/AudioEngine.h"
#include "menu/DisplayEngine.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "WandEngine.h"
#include "PackEngine.h"
#include "SmokeEngine.h"
#include "EspNowEngine.h"


ButtonReader buttonReader(WAND_FIRE_BUTTON, PACK_BUTTON, WAND_TOGGLE_BUTTON, WAND_VENT_TOGGLE_BUTTON, WAND_SPECIAL_BUTTON);

AudioEngine audioEngine;
DisplayEngine displayEngine;
EventEngine eventEngine;

Adafruit_NeoPixel wandLeds = Adafruit_NeoPixel(WAND_TOTAL_LEDS_NUMBER, WAND_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel packLeds = Adafruit_NeoPixel(PACK_TOTAL_LEDS_NUMBER, PACK_LED_PIN, NEO_GRB + NEO_KHZ800);

WandEngine wandEngine(wandLeds);
PackEngine packEngine(packLeds);
SmokeEngine smokeEngine(SMOKE_RELAY_PIN, &SMOKER_MAX_ON_TIME, &SMOKER_MIN_OFF_TIME);
EspNowEngine espNowEngine;

void setupSD()
{
    debugln("Initializing SD card...");
    if (!SD.begin(SD_CARD_CS_PIN))
    {
        debugln("initialization failed!");
        uint8_t cardType = SD.cardType();

        if (cardType == CARD_NONE)
        {
            debugln("No SD card attached");
            char *error = (char *)"No SD card attached.";
            displayEngine.produceFatal(error, ERROR_NO_SDCARD);
            initFailed = true;
            return;
        }
        return;
    }
    else
    {
        debugln("SD Initialization done.");
        uint64_t cardSize = SD.cardSize() / (1024 * 1024);
        Serial.printf("SD Card Size: %lluMB\n", cardSize);

    }
}

void onBluetoothMetadata(int code, const uint8_t *text)
{
    // Serial.printf("==> AVRC metadata rsp: attribute id 0x%x, %s\n", code, text);
    if (code == 1)
    {
        // todo tell menu system to update the audio title
        Serial.printf("Output title: %s\n", text);
    }
}

void initPreferences()
{
    Preferences prefs = getPrefs();
    bool backgroundAudio = prefs.getBool("backgroundAudio", false);
    if (backgroundAudio)
    {
        #if DEBUG == 1
            Serial.printf("background audio preference %d\n", backgroundAudio);
        #endif
        audioEngine.toggleBackground();
    }
    int theme = prefs.getInt("theme", 0);
    closePrefs();
    eventEngine.setTheme(theme);
    debugln("Theme loaded: " + String(theme));
}

// callback function that will be executed when ESP data is received
// TO DO - move this to espnowengine
void onESPEventReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
//void onESPEventReceived(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
    struct_message myData;
    memcpy(&myData, incomingData, sizeof(myData));
    byte type = incomingData[0];       // first message byte is the type of message

    //parse incoming data
     EventArgs event; // Declare the variable outside the switch
     switch (type) {
       case 0:
        // heartbeat
        break;
       case 1:
         // event
         event.eventName = String(myData.event_name);
         event.eventDetail1 = String(myData.event_detail1);
         Serial.print("Event from watch: ");
         Serial.println(myData.event_name);
         // FIXME - this should be a regular event, nothing specific to watch
         eventEngine.handleWatchEvent(event);
         break;
       default:
         // No action needed for default case
         break;
     }
  }

void initEspNow() {
    espNowEngine.setOnDataRecvCallback(onESPEventReceived);
    // espNowEngine.init();
}

void setup() {
    Serial.begin(115200);

    // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    // setup our menu, which is hardcoded :-(
    setupMenu();
    // setup our display
    displayEngine.init();
    // setup SD card
    setupSD();
    if (initFailed) {
        // if we couldn't init the SD card, we get no sound; exit now
        return;
    }

    initEspNow();

    setupBluetooth(onBluetoothMetadata, DAC_PIN_DCK, DAC_PIN_WS, DAC_PIN_DATA_OUT);

    /************* EVENT ATTACHING ***************/

    // audio engine sends events to main engine such as when an audio event ends and state needs updating
    audioEngine.attachToPlayer(&eventEngine);
    // button state is sent to event Engine
    buttonReader.attach(&eventEngine);
    // eventengine tells wand of various events/state changes
    eventEngine.attach(&wandEngine);
    // eventengine tells pack of various events/state changes
    eventEngine.attach(&packEngine);
    // eventengine tells audio of various events/state changes
    eventEngine.attach(&audioEngine);
    // eventengine tells smoke engine its time to go
    eventEngine.attach(&smokeEngine);
    // smoke engine tells event engine that smoke is off
    smokeEngine.attach(&eventEngine);
    // pack would tell smoke engine that it has overheated
    packEngine.attach(&smokeEngine);
    // pack would tell audio engine that is has overheated, in order to change the sounds
    packEngine.attach(&audioEngine);

    // espnow engine needs to know when bluetooth is turned on since they are not compatible :sadface:
    eventEngine.attach(&espNowEngine);

    // to do - read last volume saved?
    audioEngine.init(1.0f);
    wandEngine.init();
    packEngine.init();
    smokeEngine.init();

    initPreferences();

    debugln("Setup done.");

}

void loop()
{
    if (initFailed)
    {
        return;
    }
    // tell each of our engines to do some work
    buttonReader.work();
    audioEngine.work();
    displayEngine.work();
    smokeEngine.work();
    wandEngine.work();
    packEngine.work();
    eventEngine.work();
}
