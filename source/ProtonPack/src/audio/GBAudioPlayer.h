#pragma once

#include "../common.h"
#include "Arduino.h"
#include "Subject.h"
#include "Observer.h"
#include "EventArgs.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
// #include "AudioTools/AudioCodecs/CodecWAV.h"
// #include "MP3DecoderHelix.h"
#include <SPI.h>
#include <SD.h>


class GBAudioPlayer : public Subject<EventArgs>, public AudioInfoSupport
{
   public:
    GBAudioPlayer();
    void init(float volume);

    virtual AudioInfo audioInfo() override;
    virtual void setAudioInfo(AudioInfo info) override;
    void sendEvent(EventArgs args);
    void sendSimpleEvent(const char *event);
    void stopAudio();
    void playFile(String filename);
    void setFollowEvent(String event);
    void setVolume(float volume);
    void shouldSendVolumeEvents(bool send);
    float getVolume();
    void playFile(String filename, uint32_t startPosition);
    void audioLoop();
    uint32_t getCurrentPosition();
    void stopPlaying(bool suppressEvent);

  private:
    I2SStream out;
    VolumeStream volume_out; // Volume control
    EncodedAudioOutput out_decoding; // Decoding stream
    FadeStream fade;
    AudioInfo info;
    MP3DecoderHelix decoder;
    VolumeMeter volume_meter_out;
    // WAVDecoder wavDecoder;
    StreamCopy copier;
    File audioFile;
    bool playing = false;
    bool audioInitialized = false;
    float current_volume = -1.0f;
    bool sendVolumeEvents = false;
    String lastPlayed = "";
    String postEvent = "";
    void notifyStopped();
    void sendVolumeEvent(float volume);
    void dataCallback(MP3FrameInfo &info, short *pcm_buffer, size_t len, void* ref);

};