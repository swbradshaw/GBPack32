#pragma once

#include "../common.h"
#include "Arduino.h"
#include "Observer.h"
#include "EventArgs.h"
#include "AudioTheme.cpp"
#include "GBAudioPlayer.h"

class AudioEngine : public Observer<EventArgs>, public Subject<EventArgs>
{
  public:
    AudioEngine();
    void setTheme(int themeNumber);
	  void notify(EventArgs args) override;
    float getVolume();
    void setVolume(float volume);
    void shutdownAudioEngine();
    void init(float volume);
    void work();
    void playBackground(String filename);
    void playPackSound(String filename);
    void toggleBackground();
    bool isBackgroundAudioOn();
    void attachToPlayer(Observer<EventArgs> *observer);
    bool isAudioPlaying();
    void dispatchEvent(EventArgs event);
    // static void staticPlayPostStreamSound(AudioEngine *obj) {
  private:
    // BluetoothEngine *btEngine;

    uint8_t state = 0;
    GBAudioPlayer player;
    bool audioPlaying = false;
    bool playingMusic = false;
    bool packOn = false;
    bool wandOn = false;

    // vent variable is used to keep track of when pack has overheated, so that we ignore when user releases the proton stream button
    bool venting = false;
    bool userWantsBackgroundSound = false;
    bool backgroundPlaying = false;
    bool backgroundOverride = false;
    bool packSoundPlaying = false;
    bool audioInitialized = false;
    AudioTheme theme;
    uint32_t lastBackgroundPosition = 0;
    String lastBackgroundFilename = "";

    void cycleBackground();
    void resetBackground();

    void startMainBackgroundIfNotPlaying();
    void startMainBackgroud();
    void handlePlayEvent(EventArgs args);
    void cancelBackground();
    void handleAudioEvent(EventArgs args);
    void handleFollowUpEvent(String eventName);
    void handlePackStateEvent(EventArgs args);
    void handleWandEvent(EventArgs args);
    void handleButtonEvent(EventArgs args);
    void handleOverheatEvent(EventArgs args);

};
