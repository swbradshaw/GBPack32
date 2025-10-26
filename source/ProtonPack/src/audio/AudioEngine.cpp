#include "AudioEngine.h"

AudioEngine::AudioEngine() {
    player.attach(this);
    theme.attach(this);
}

void AudioEngine::setTheme(int themeNumber) {
    theme.setTheme(themeNumber);
}
void AudioEngine::notify(EventArgs args)
{
    if (args.eventName.startsWith("EVENT_PACK_")) {
        handlePackStateEvent(args);
    }
    if (args.eventName.startsWith("EVENT_WAND_")) {
        handleWandEvent(args);
    }
    if (args.eventName.startsWith("EVENT_AUDIO")) {
        handleAudioEvent(args);
    }
    if (args.eventName == EVENT_CUSTOM_PACK_STARTUP_1) {
        // TODO - move this to a new PLAY event that takes an argument of which file to play
        playPackSound("/audio/pack-sounds/special/01-havent-had-successful-test.mp3");
        player.setFollowEvent(EVENT_BUTTON_PACK);
    }
    if (args.eventName.startsWith("EVENT_OVERHEAT")) {
        handleOverheatEvent(args);
    }
    if (args.eventName.equals(EVENT_VENT_PACK)) {
        handleOverheatEvent(args);
    }
    if (args.eventName.equals(EVENT_PLAY_SOUND)) {
        handlePlayEvent(args);
    }
    if (args.eventName.equals(EVENT_THEME_CHANGE)) {
        setTheme(args.eventDetail1.toInt());
        resetBackground();
        startMainBackgroud();
    }
    if (args.eventName.equals(EVENT_AUDIO_VOLUME_CHANGE)) {
        setVolume(args.eventDetail1.toFloat());
    }
}

float AudioEngine::getVolume() {
    return player.getVolume();
}
void AudioEngine::setVolume(float volume) {
    player.setVolume(volume);
    EventArgs args;
    args.eventName = EVENT_AUDIO_VOLUME_CHANGED;
    args.eventDetail1 = String(volume);
    notify(args);
}

void AudioEngine::shutdownAudioEngine() {
    player.stopPlaying(true);
    player.stopAudio();
    audioInitialized = false;
}
void AudioEngine::init(float volume) {
    debugln("AudioEngine::init");
    audioInitialized = true;
    player.init(volume);

}
void AudioEngine::work() {
    player.audioLoop();
    theme.work();
}
void AudioEngine::dispatchEvent(EventArgs event) {
    // hack to allow menu system to send event
    notify(event);
    player.sendEvent(event);
}
void AudioEngine::playBackground(String filename) {
    if (audioInitialized) {
        packSoundPlaying = false;
        backgroundPlaying = true;
        lastBackgroundFilename = filename;
        player.playFile(filename, lastBackgroundPosition);
    }
}
void AudioEngine::playPackSound(String filename) {

    debugln("playPackSound");
    if ((audioInitialized) && (filename.length() > 0))  {
        if (backgroundPlaying) {
            // save position
            lastBackgroundPosition = player.getCurrentPosition();
        }
        packSoundPlaying = true;
        backgroundPlaying = false;
        player.playFile(filename);
    }
}
void AudioEngine::toggleBackground() {
    userWantsBackgroundSound = !userWantsBackgroundSound;
    if (!userWantsBackgroundSound) {
        backgroundPlaying = false;
        lastBackgroundPosition = 0;
        player.stopPlaying(true);
    } else {
        if (packOn) {
            startMainBackgroud();
        }
    }
}
bool AudioEngine::isBackgroundAudioOn() {
    return userWantsBackgroundSound;
}
void AudioEngine::attachToPlayer(Observer<EventArgs> *observer) {
    player.attach(observer);
}

bool AudioEngine::isAudioPlaying() {
    return backgroundPlaying  || packSoundPlaying;
}
void AudioEngine::cycleBackground() {
    theme.advanceBackground();
    lastBackgroundFilename = "";
    lastBackgroundPosition = 0;
    startMainBackgroud();
}
void AudioEngine::resetBackground() {
    theme.resetBackground();
    lastBackgroundFilename = "";
    lastBackgroundPosition = 0;
}

void AudioEngine::startMainBackgroundIfNotPlaying() {
    if (!backgroundPlaying) {
        startMainBackgroud();
    }
}
void AudioEngine::startMainBackgroud() {
    bool shouldPlay = userWantsBackgroundSound && ((packOn) || (playingMusic));
    if (shouldPlay) {
    //if (isBackgroundAudioOn()) {
        // debugln("User wants background");
        if (lastBackgroundFilename.length() > 0) {
            debugln("playing last background file");
            playBackground(lastBackgroundFilename);
        } else {
            debugln("playing backgroundsong");
            playBackground(theme.getBackgroundSong());
        }
    }
}
void AudioEngine::handlePlayEvent(EventArgs args) {
    playPackSound(args.eventDetail1);
}
void AudioEngine::cancelBackground() {
    lastBackgroundPosition = 0;
    lastBackgroundFilename = "";
    player.stopPlaying(true);
    playingMusic = false;
}

void AudioEngine::handleOverheatEvent(EventArgs args) {
    if (args.eventName == EVENT_OVERHEAT) {
        int stage = args.eventDetail1.toInt();
        // we change audio when stage is 3, 4, or 5
        switch(stage) {
            case 3:
                // stream + beeping
                playPackSound(theme.getStreamFireWithOverheatSound());
                break;
            case 4:
                // overheat has occurred, start beeping
                venting = true;
                playPackSound(theme.getOverheatSound());
                break;
            case 5:
                // venting
                venting = true;
                playPackSound(theme.getVentShortSound());
                player.setFollowEvent(EVENT_OVERHEAT_COMPLETE);
                break;
            default:
                venting = false;
                break;
        }
    }
    if (args.eventName == EVENT_VENT_PACK) {
        playPackSound(theme.getVentShortSound());
    }
    if (args.eventName == EVENT_OVERHEAT_COMPLETE) {
        venting = false;
        startMainBackgroud();
    }
  }

void AudioEngine::handleAudioEvent(EventArgs args) {

    // debugln("handleAudioEvent in AudioEngine: " + args.eventName);
    if (args.eventName == EVENT_AUDIO_INIT) {
        startMainBackgroud();
        return;
    }
    if (args.eventName == EVENT_AUDIO_SEND_VOLUME_EVENTS) {
        player.shouldSendVolumeEvents(args.eventDetail1.toInt() > 0);
        return;
    }
    if (args.eventName == EVENT_AUDIO_STOPPED_PLAYING) {
        handleFollowUpEvent(args.emitFollowUpEvent);
        if (playingMusic) {
            // advance to next track - only if we were NOT playing a pack sound?
            if (packSoundPlaying) {
                // we were playing a pack sound, we don't want to advance the background - we just want to continue where we left off
                packSoundPlaying = false;
                startMainBackgroud();
            } else {
                // clear out last used song and position
                lastBackgroundPosition = 0;
                lastBackgroundFilename = "";
                cycleBackground();
            }

        } else {
            if (backgroundPlaying) {
                // we are done playing the current background song. now what?
                debugln("background done playing");
                // clear out last used song and position
                lastBackgroundPosition = 0;
                lastBackgroundFilename = "";
            }
            backgroundPlaying = false;
            packSoundPlaying = false;
            if ((userWantsBackgroundSound) && (packOn)) {
                playBackground(theme.getPackBackground());
            }
        }

        // if (packOn || (!packOn && lastBackgroundPosition > 0)) {
        //     // only resume the background if the pack is on, or if we were previously playing another clip while the pack was off
        //     startMainBackgroud();
        // }
    }
    if (args.eventName == EVENT_AUDIO_TOGGLE_BACKGROUND) {
        toggleBackground();
    }
    if (args.eventName == EVENT_AUDIO_SHUFFLE_BACKGROUND) {
        userWantsBackgroundSound = true;
        playingMusic = true;
        cycleBackground();
    }
    if (args.eventName == EVENT_AUDIO_STOP_BACKGROUND) {
        cancelBackground();
        userWantsBackgroundSound = false;
    }
    if (args.eventName == EVENT_AUDIO_RESET_BACKGROUND) {
        cancelBackground();
        // pack is on; lets just reset back to the main background (pack humming)
        theme.resetBackground();
        playBackground(theme.getPackBackground());
    }
    if (args.eventName == EVENT_AUDIO_PLAY_SOUND_CLIP) {
        playPackSound(theme.getSoundClip());
        player.setFollowEvent(EVENT_AUDIO_STOP_SOUND_CLIP);
    }
    if (args.eventName == EVENT_AUDIO_PLAY_MOVIE_QUOTE) {
        playPackSound(theme.getMovieQuote());
    }
}

void AudioEngine::handleFollowUpEvent(String eventName) {
    // possible events from audio ending
    debugln("AudioEngine handleFollowUpEvent: " + eventName);

    if (eventName == EVENT_WAND_IDLE) {
        wandOn = true;
    }
    if (eventName == EVENT_WAND_OFF) {
        wandOn = false;
    }
    if (eventName == EVENT_PACK_IDLE) {
        packOn = true;
    }
    if (eventName == EVENT_PACK_OFF) {
        packOn = false;
        userWantsBackgroundSound = false;
    }
}
void AudioEngine::handlePackStateEvent(EventArgs args) {
    if (args.eventName == EVENT_PACK_BOOTING) {
        debugln("AudioEngine::handlePackStateEvent: EVENT_PACK_BOOTING  followed up by " + args.emitFollowUpEvent);
        userWantsBackgroundSound = true;
        playPackSound(theme.getPackPowerOn());
        player.setFollowEvent(args.emitFollowUpEvent);
    }
    if (args.eventName == EVENT_PACK_SHUTTING_DOWN) {
        // pack shutting off
        debugln("AudioEngine::handlePackStateEvent: EVENT_PACK_SHUTTING_DOWN  followed up by " + args.emitFollowUpEvent);
        playPackSound(theme.getPackPowerOff());
        userWantsBackgroundSound = false;
        resetBackground();
        player.setFollowEvent(args.emitFollowUpEvent);
    }
}
void AudioEngine::handleWandEvent(EventArgs args) {

    debugln("AudioEngine::handleWandEvent: " + args.eventName + "/" + args.eventDetail1 + "/" + args.eventDetail2);
    if (args.eventName == EVENT_WAND_BOOTING) {
        userWantsBackgroundSound = true;
        bool packOn = args.eventDetail1.equals("PACKON");
        playPackSound(theme.getWandPowerOn(packOn));
        player.setFollowEvent(args.emitFollowUpEvent);
    }
    if (args.eventName == EVENT_WAND_SHUTTING_DOWN) {
        // wand shutting off
        playPackSound(theme.getWandPowerOff());
        player.setFollowEvent(args.emitFollowUpEvent);
    }
    if (args.eventName == EVENT_WAND_FIRING_START) {
        playPackSound(theme.getStreamFireSound1());
    }
    if (args.eventName == EVENT_WAND_FIRING_STOP) {
        // user stopped firing, do something if we are't venting/overheated
        if (!venting) {
            String postSound = theme.getPostStreamSound();
            playPackSound(postSound);
        }
    }
}