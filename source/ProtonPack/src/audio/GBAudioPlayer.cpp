#include "GBAudioPlayer.h"

#include "../common.h"

typedef void (*MP3DataCallback)(MP3FrameInfo &info, short *pcm_buffer, size_t len, void *ref);

GBAudioPlayer::GBAudioPlayer() { AudioLogger::instance().begin(Serial, AudioLogger::Error); }

void GBAudioPlayer::init(float volume) {
  fade.setAudioInfo(info);

  // Setup the pins for pcm5102
  auto cfg_output = out.defaultConfig(TX_MODE);
  cfg_output.pin_bck = DAC_PIN_DCK;        // BCK
  cfg_output.pin_ws = DAC_PIN_WS;          // LCK
  cfg_output.pin_data = DAC_PIN_DATA_OUT;  // DIN
  out.begin(cfg_output);

  fade.setOutput(out);

  volume_out.setOutput(fade);

  volume_meter_out.setOutput(volume_out);

  out_decoding.setOutput(volume_meter_out);
  out_decoding.setDecoder(&decoder);
  // wavDecoder.addNotifyAudioChange(*this);
  decoder.addNotifyAudioChange(*this);

  // declare a callback of type MP3DataCallback
  // MP3DataCallback callback = [](MP3FrameInfo &info, short *pcm_buffer, size_t len, void *ref) {
  //     static_cast<GBAudioPlayer*>(ref)->dataCallback(info, pcm_buffer, len, ref);
  // };
  // decoder.driver()->setDataCallback(callback);

  setVolume(volume);

  audioInitialized = true;
  sendSimpleEvent(EVENT_AUDIO_INIT);
}

void GBAudioPlayer::dataCallback(MP3FrameInfo &info, short *pcm_buffer, size_t len, void *ref) {
  Serial.print(" ");
  Serial.print(pcm_buffer[0]);
  //     for (size_t i=0; i<len; i+=info.nChans){
  //         for (int j=0;j<info.nChans;j++){
  //             Serial.print(pcm_buffer[i+j]);
  //             Serial.print(" ");
  //         }
  //         Serial.println();
  //     }
}

AudioInfo GBAudioPlayer::audioInfo() { return info; }

/// Updates the audio info in the related objects
void GBAudioPlayer::setAudioInfo(AudioInfo info) {
  this->info = info;
  // notify volume
  volume_out.setAudioInfo(info);
  volume_meter_out.setAudioInfo(info);
  fade.setAudioInfo(info);
};

void GBAudioPlayer::sendSimpleEvent(const char *event) {
  EventArgs args;
  args.eventName = event;
  Subject::notify(args);
}
void GBAudioPlayer::sendEvent(EventArgs args) { Subject::notify(args); }

void GBAudioPlayer::stopAudio() {
  // stop audio engine
  out.end();
  audioInitialized = false;
  sendSimpleEvent(EVENT_AUDIO_SHUTDOWN);
  debugln("Audio engine shutdown");
}

void GBAudioPlayer::playFile(String filename) { playFile(filename, 0); }
void GBAudioPlayer::setFollowEvent(String event) { postEvent = event; }
void GBAudioPlayer::setVolume(float volume) {
  if (volume > 1.0f) {
    volume = volume / 100.0f;
  }
  if (volume >= 0.0f && volume <= 1.0f) {
    if (abs(volume - current_volume) > 0.01f) {
      volume_out.setVolume(volume);
      current_volume = volume;
    }
  }
}
float GBAudioPlayer::getVolume() { return current_volume; }

void GBAudioPlayer::shouldSendVolumeEvents(bool send) { sendVolumeEvents = send; }

void GBAudioPlayer::playFile(String filename, uint32_t startPosition) {
  if (audioInitialized) {
    stopPlaying(true);

    debugln("Playing " + filename);
    // if (filename.endsWith(".mp3")) {
    //     out_decoding.setDecoder(&decoder);
    // // } else if (filename.endsWith(".wav")) {
    // //     out_decoding.setDecoder(&wavDecoder);
    // } else {
    //     debugln("Unsupported file format");
    //     return;
    // }
    audioFile = SD.open(filename);
    lastPlayed = filename;
    if (startPosition > 0) {
      audioFile.seek(startPosition);
    }

    out_decoding.begin();
    copier.begin(out_decoding, audioFile);
    playing = true;

    EventArgs args;
    args.eventName = EVENT_AUDIO_STARTED_PLAYING;
    args.eventDetail1 = filename;
    Subject::notify(args);
  }
}
void GBAudioPlayer::audioLoop() {
  if (playing) {
    if (copier.isActive()) {
      if (!copier.copy()) {
        stopPlaying(false);
      } else {
        sendVolumeEvent(volume_meter_out.volumePercent());
      }
    }
  }
}

void GBAudioPlayer::sendVolumeEvent(float volume) {
  if (sendVolumeEvents) {
    EventArgs volArgs;
    volArgs.eventName = EVENT_AUDIO_VOLUME;
    volArgs.eventDetail1 = String(volume);
    Subject::notify(volArgs);
  }
}

uint32_t GBAudioPlayer::getCurrentPosition() {
  if (playing) {
    return audioFile.position();
  } else {
    return 0;
  }
}
void GBAudioPlayer::stopPlaying(bool suppressEvent) {
  if (playing) {
    debugln("stopPlaying");

    playing = false;
    delay(20);
    copier.end();
    audioFile.close();
    out_decoding.end();
    sendVolumeEvent(0.0);

    if (!suppressEvent) {
      notifyStopped();
    }
  }
}

void GBAudioPlayer::notifyStopped() {
  EventArgs args;
  args.eventName = EVENT_AUDIO_STOPPED_PLAYING;
  args.eventDetail1 = lastPlayed;
  args.emitFollowUpEvent = postEvent;
  postEvent = "";
  Subject::notify(args);
}
