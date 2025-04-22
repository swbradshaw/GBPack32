#include "DisplayEngine.h"


DisplayEngine::DisplayEngine() {
}

void DisplayEngine::notify(EventArgs args){

    debugln("DisplayEngine: notify");
    debugln(args.eventName);

    if (args.eventName.startsWith("EVENT_AUDIO")) {
        handleAudioEventForDisplay(args);
    }
}
void DisplayEngine::init() {
    u8g2.begin();
    u8g2.setBitmapMode(1);

    debugln("Display initialized.");


    // Enable Screensaver (screensaver menu function, time to activate in ms)
    LCDML.SCREEN_enable(mFunc_screensaver, 20000); // set to 10 seconds
    // start in screen saver
    LCDML.OTHER_jumpToFunc(mFunc_screensaver);

}
void DisplayEngine::work() {
    LCDML.loop();
}
void DisplayEngine::produceFatal(char *message, uint8_t code) {
    // show Fatal message on screen, assuming screen is working
    u8g2.setFont(u8g_font_6x10);
    u8g2.drawStr(5, 10, message);
    u8g2.nextPage();
}


void DisplayEngine::handleAudioEventForDisplay(EventArgs args) {

    debugln("DisplayEngine: Audio event received");
    debugln(args.eventName);

    // when the audio plays, we need to send a custom event that tells the screen saver to stop
    // this will save CPU cycles
    if (args.eventName.equals(EVENT_AUDIO_STARTED_PLAYING)) {
        // tell display that a button was pressed
        //  audioPlaying = true;
        // LCDML.REG_set(_LCDML_REG_CTRL__events, 1);
    }
    if (args.eventName.equals(EVENT_AUDIO_STOPPED_PLAYING)) {
        // clear custom event so our screen saver can turn back on
        // LCDML.REG_resetAll(_LCDML_REG_CTRL__events);
        // audioPlaying = false;
    }

}
