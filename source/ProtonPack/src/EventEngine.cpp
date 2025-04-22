#include "EventEngine.h"
#define FASTLED_INTERNAL // remove annoying pragma messages

// The glue that makes all of the components talk to each other
// This listens for input events (button events, etc) and dispatches other events

EventEngine::EventEngine() {
    packOn = false;
    wandOn = false;
    wandFiring = false;
}

void EventEngine::notify(EventArgs args) {
    handleEvent(args);
}

void EventEngine::work() {
 // no work here
}

void EventEngine::handleEvent(EventArgs args) {
    if (args.eventName.startsWith("BUTTON")) {
        handlePackButtonEvent(args);
        handleWandButtonEvent(args);
        handleFireButtonEvent(args);
    }
    // we handle audio events (mainly when the sound stops) so that we can dispatch follow up events
    if (args.eventName.startsWith("EVENT_AUDIO")) {
        handleAudioEvent(args);
    }
    if (args.eventName == EVENT_CUSTOM_PACK_STARTUP_1) {
        Subject::notify(args);
    }
    if (args.eventName.equals(EVENT_THEME_CHANGE)) {
        handleThemeChangeEvent(args);
    }
    if (args.eventName.startsWith("EVENT_SMOKE_")) {
        // rebroadcast (event came from menu system) - to be handle by smoke engine and pack engine (turn on vent light)
        Subject::notify(args);
    }
}

void EventEngine::handleThemeChangeEvent(EventArgs args) {

    // save theme
    Preferences prefs = getPrefs();
    prefs.putInt("theme", args.eventDetail1.toInt());
    closePrefs();
    debugln("Saved theme to prefs: "+args.eventDetail1);

    setTheme(args.eventDetail1.toInt());
}

void EventEngine::setTheme(int theme) {
    // broadcast a theme change
     EventArgs newArgs;
    newArgs.eventName = EVENT_THEME_CHANGE;
    newArgs.eventDetail1 = String(theme);
    Subject::notify(newArgs);

}

void EventEngine::handleAudioEvent(EventArgs args) {
    if (args.eventName == EVENT_AUDIO_STOPPED_PLAYING) {
        // an audio clip has stopped playing.

        // if there is a follow up event based on this audio finishing, dispatch this now.
        if (args.emitFollowUpEvent != "") {
            handleFollowUpEvent(args.emitFollowUpEvent);
            EventArgs newArgs;
            newArgs.eventName = args.emitFollowUpEvent;
            Subject::notify(newArgs);

        }
    } else {
        Subject::notify(args);
    }
}
void EventEngine::handleFollowUpEvent(String eventName) {
    // possible events from audio ending
    if (eventName == EVENT_WAND_IDLE) {
        wandBooting = false;
        wandOn = true;
    }
    if (eventName == EVENT_WAND_OFF) {
        wandBooting = false;
        wandOn = false;
    }
    if (eventName == EVENT_PACK_BOOTING) {
        packBooting = true;
        packOn = false;
    }
    if (eventName == EVENT_PACK_IDLE) {
        packBooting = false;
        packOn = true;
    }
    if (eventName == EVENT_PACK_OFF) {
        packBooting = false;
        packOn = false;
    }
    if (eventName == EVENT_BUTTON_PACK) { // handle case for special start-up
        EventArgs args;
        args.eventName = EVENT_BUTTON_PACK;
        args.eventDetail1 = "SINGLE_CLICK";
        handlePackButtonEvent(args);
    }
}

void EventEngine::handleWandButtonEvent(EventArgs args) {
    if (args.eventName == EVENT_BUTTON_WAND) {
        if (args.eventDetail1 == "SINGLE_CLICK") {
            // don't do anything while either the pack or the wand is booting
            if ((!wandBooting) && (!packBooting)) {
                if (!wandOn) {
                    wandBooting = true;
                    EventArgs args;
                    args.eventName = EVENT_WAND_BOOTING;
                    args.eventDetail1 = packOn ? "PACKON" : "PACKOFF";
                    args.emitFollowUpEvent = EVENT_WAND_IDLE;
                    Subject::notify(args);
                } else {
                    EventArgs args;
                    args.eventName = EVENT_WAND_SHUTTING_DOWN;
                    args.eventDetail1 = packOn ? "PACKON" : "PACKOFF";
                    args.emitFollowUpEvent = EVENT_WAND_OFF;
                    Subject::notify(args);
                }
            }

        }
    }


    // we use this to change background audio
    if (args.eventName == EVENT_BUTTON_AUDIO) {
        if (args.eventDetail1 == "LONG_CLICK") {
            if (args.eventDetail2 == "RELEASED") {
                if (packOn) {
                    EventArgs audioEvent;
                    audioEvent.eventName = EVENT_AUDIO_RESET_BACKGROUND;
                    Subject::notify(audioEvent);
                } else {
                    EventArgs audioEvent;
                    audioEvent.eventName = EVENT_AUDIO_STOP_BACKGROUND;
                    Subject::notify(audioEvent);
                }

            }
        }
        if (args.eventDetail1 == "SINGLE_CLICK") {
            debugln("SINGLE_CLICK EVENT_BUTTON_AUDIO");
            EventArgs audioEvent;
            audioEvent.eventName = EVENT_AUDIO_SHUFFLE_BACKGROUND;
            Subject::notify(audioEvent);
        }
    }

    // this is our vent toggle on the wand
    // TODO - Make this dependant on the theme
    // ie, flipping switch could actually just trigger smoke instead of toggling overheat sequence
    if (args.eventName == EVENT_BUTTON_VENT) {
        wandVentOn = (args.eventDetail1 == "ON");

        // Notify other components that the vent is on or off
        EventArgs args;
        args.eventName = wandVentOn ? EVENT_VENT_WAND_ON : EVENT_VENT_WAND_OFF;
        Subject::notify(args);

        // send another event to test the smoke
        // if (wandVentOn) {
        //     EventArgs args;
        //     args.eventName = EVENT_SMOKE_ON;
        //     args.eventDetail1 = "30000";
        //     Subject::notify(args);
    }
}

void EventEngine::handleWatchEvent(EventArgs event) {
    // handle events from watch
    if ((event.eventName == EVENT_WATCH_TURN_PACK_ON) || (event.eventName == EVENT_WATCH_TURN_PACK_OFF)) {
        EventArgs args;
        args.eventName = EVENT_BUTTON_PACK;
        args.eventDetail1 = "SINGLE_CLICK";
        handlePackButtonEvent(args);
    } else if ((event.eventName == EVENT_WATCH_TURN_WAND_ON) || (event.eventName == EVENT_WATCH_TURN_WAND_OFF)) {
        EventArgs args;
        args.eventName = EVENT_BUTTON_WAND;
        args.eventDetail1 = "SINGLE_CLICK";
        handleWandButtonEvent(args);
    } else if (event.eventName == EVENT_VENT_PACK) {
        // start smoke and play audio clip
        EventArgs smokeEvent;
        smokeEvent.eventName = EVENT_SMOKE_ON;
        smokeEvent.eventDetail1 = "6000"; // 6 seconds
        Subject::notify(smokeEvent);
        Subject::notify(event);

    } else if (event.eventName == EVENT_BUTTON_AUDIO) {
        if (event.eventDetail1 == "LONG_CLICK") {
            if (packOn) {
                EventArgs audioEvent;
                audioEvent.eventName = EVENT_AUDIO_RESET_BACKGROUND;
                Subject::notify(audioEvent);
            } else {
                EventArgs audioEvent;
                audioEvent.eventName = EVENT_AUDIO_STOP_BACKGROUND;
                Subject::notify(audioEvent);
            }
        }
    } else {
        handleEvent(event);
    }
}

void EventEngine::handlePackButtonEvent(EventArgs args) {
    if (args.eventName == EVENT_BUTTON_PACK) {
        if (args.eventDetail1 == "LONG_CLICK") {
            // reboot esp32
            debugln("restart ESP");
            ESP.restart();
        } else {
            if ((!wandBooting) && (!packBooting)) {
                if (!packOn) {
                    packBooting = true;
                    EventArgs args;
                    args.eventName = EVENT_PACK_BOOTING;
                    args.emitFollowUpEvent = EVENT_PACK_IDLE;
                    Subject::notify(args);
                } else {
                    packBooting = false;
                    packShuttingDown = true;
                    EventArgs args;
                    args.eventName = EVENT_PACK_SHUTTING_DOWN;
                    args.emitFollowUpEvent = EVENT_PACK_OFF;
                    Subject::notify(args);
                }
            }

        }
    }
}

void EventEngine::handleFireButtonEvent(EventArgs args) {

    // single press - random movie sound clip
    // double press - random movie quote
    // long press - fire wand!
     if (args.eventName == EVENT_BUTTON_FIRE) {
        if (args.eventDetail1 == "SINGLE_CLICK") {
            EventArgs audioEvent;
            audioEvent.eventName = EVENT_AUDIO_PLAY_SOUND_CLIP;
            Subject::notify(audioEvent);
        }
        if (args.eventDetail1 == "DOUBLE_CLICK") {
            EventArgs audioEvent;
            audioEvent.eventName = EVENT_AUDIO_PLAY_MOVIE_QUOTE;
            Subject::notify(audioEvent);
        }
        if (args.eventDetail1 == "LONG_CLICK") {
            // doesn't do anything unless the pack and wand is on
            if ((packOn) && (wandOn)) {
                if (args.eventDetail2 == "RELEASED") {
                    wandFiring = false;
                    EventArgs args;
                    args.eventName = EVENT_WAND_FIRING_STOP;
                    Subject::notify(args);
                } else {
                    wandFiring = true;
                    EventArgs args;
                    args.eventName = EVENT_WAND_FIRING_START;
                    Subject::notify(args);
                }
            }
        }
     }
}
