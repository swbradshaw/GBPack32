#include "ButtonReader.h"


ButtonReader::ButtonReader(byte fireGPIO, byte packGPIO, byte wandGPIO, byte ventGPIO, byte specialGPIO) {
        // setup?
        fireButton = new PinButton(fireGPIO,INPUT_PULLUP);
        packButton = new PinButton(packGPIO, INPUT_PULLUP);
        wandButton = new PinButton(wandGPIO, INPUT_PULLUP);
        ventButton = new PinButton(ventGPIO, INPUT_PULLUP);
        musicButton = new PinButton(specialGPIO, INPUT_PULLUP);
}

void ButtonReader::work() {
    // initialize
    if (!started) {
        startTime = millis();
        started = true;
        lastRead = millis();
    }
    if ((lastRead - startTime) > 1000) {
        readButtons();
    }
    lastRead = millis();
}

void ButtonReader::readButtons() {
    // Read hardware pin, convert to click events
    fireButton->update();
    packButton->update();
    wandButton->update();
    ventButton->update();
    musicButton->update();

    if (ventButton->isClick()) {
        debugln("ventButton isClick");
        EventArgs args;
        args.eventName = EVENT_BUTTON_VENT;
        args.eventDetail1 = "ON";
        Subject::notify(args);
    }

    if (ventButton->isReleased()) {
        debugln("ventButton isReleased");
        EventArgs args;
        args.eventName = EVENT_BUTTON_VENT;
        args.eventDetail1 = "OFF";
        Subject::notify(args);
    }

    if (wandButton->isClick()) {
        debugln("wandButton singleclick");
        EventArgs args;
        args.eventName = EVENT_BUTTON_WAND;
        args.eventDetail1 = "SINGLE_CLICK";
        Subject::notify(args);
    }
    if (packButton->isClick()) {
        debugln("packButton singleclick");
        EventArgs args;
        args.eventName = EVENT_BUTTON_PACK;
        args.eventDetail1 = "SINGLE_CLICK";
        Subject::notify(args);
    }
    if (packButton->isLongClick()) {
        debugln("packButton isLongClick");
        EventArgs args;
        args.eventName = EVENT_BUTTON_PACK;
        args.eventDetail1 = "LONG_CLICK";
        Subject::notify(args);
    }
    if (musicButton->isLongClick()) {
        lastSpecialClick = 3;

        debugln("musicButton isLongClick");

        EventArgs args;
        args.eventName = EVENT_BUTTON_AUDIO;
        args.eventDetail1 = "LONG_CLICK";
        Subject::notify(args);
    }
    if (musicButton->isSingleClick()) {
        lastSpecialClick = 1;

        debugln("musicButton singleclick");

        EventArgs args;
        args.eventName = EVENT_BUTTON_AUDIO;
        args.eventDetail1 = "SINGLE_CLICK";
        Subject::notify(args);
    }

    if (musicButton->isDoubleClick()) {
        lastSpecialClick = 2;
        EventArgs args;
        args.eventName = EVENT_BUTTON_AUDIO;
        args.eventDetail1 = "DOUBLE_CLICK";
        Subject::notify(args);
    }
    if (musicButton->isReleased()) {
        if (lastSpecialClick == 3) {
            EventArgs args;
            args.eventName = EVENT_BUTTON_AUDIO;
            args.eventDetail1 = "LONG_CLICK";
            args.eventDetail2 = "RELEASED";
            Subject::notify(args);
            lastSpecialClick = 0;
        }
    }


    if (fireButton->isLongClick()) {

        debugln("fireButton isLongClick");

        lastFireClick = 3;
        EventArgs args;
        args.eventName = EVENT_BUTTON_FIRE;
        args.eventDetail1 = "LONG_CLICK";
        Subject::notify(args);
    }
    if (fireButton->isSingleClick()) {

        debugln("fireButton isSingleClick");

        lastFireClick = 1;
        EventArgs args;
        args.eventName = EVENT_BUTTON_FIRE;
        args.eventDetail1 = "SINGLE_CLICK";
        Subject::notify(args);
    }
    if (fireButton->isDoubleClick()) {
        lastFireClick = 2;
        // Only triggers on a single, short click (i.e. not
        // on the first click of a double-click, nor on a long click).
        EventArgs args;
        args.eventName = EVENT_BUTTON_FIRE;
        args.eventDetail1 = "DOUBLE_CLICK";
        Subject::notify(args);
    }
    if (fireButton->isReleased()) {
        if (lastFireClick == 3) {
            EventArgs args;
            args.eventName = EVENT_BUTTON_FIRE;
            args.eventDetail1 = "LONG_CLICK";
            args.eventDetail2 = "RELEASED";
            Subject::notify(args);
            lastFireClick = 0;
        }
    }
    }

