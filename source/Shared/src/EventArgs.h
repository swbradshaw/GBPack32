#ifndef EVENT_ARGS_CLASS
#define EVENT_ARGS_CLASS
#include <Arduino.h>

// Button events
static constexpr auto EVENT_BUTTON_FIRE = "BUTTON_FIRE";
static constexpr auto EVENT_BUTTON_WAND = "BUTTON_WAND";
static constexpr auto EVENT_BUTTON_PACK = "BUTTON_PACK";
static constexpr auto EVENT_BUTTON_VENT = "BUTTON_VENT";
static constexpr auto EVENT_BUTTON_AUDIO = "BUTTON_AUDIO";

// Watch events
static constexpr auto EVENT_WATCH_TURN_PACK_ON = "EVENT_WATCH_TURN_PACK_ON";
static constexpr auto EVENT_WATCH_TURN_PACK_OFF = "EVENT_WATCH_TURN_PACK_OFF";
static constexpr auto EVENT_WATCH_TURN_WAND_ON = "EVENT_WATCH_TURN_WAND_ON";
static constexpr auto EVENT_WATCH_TURN_WAND_OFF = "EVENT_WATCH_TURN_WAND_OFF";

// Audio events
static constexpr auto EVENT_AUDIO_VOLUME_CHANGE = "EVENT_AUDIO_VOLUME_CHANGE";
static constexpr auto EVENT_AUDIO_VOLUME_CHANGED = "EVENT_AUDIO_VOLUME_CHANGED";
static constexpr auto EVENT_AUDIO_INIT = "EVENT_AUDIO_INIT";
static constexpr auto EVENT_AUDIO_SHUTDOWN = "EVENT_AUDIO_SHUTDOWN";
static constexpr auto EVENT_AUDIO_STOPPED_PLAYING = "EVENT_AUDIO_STOPPED_PLAYING";
static constexpr auto EVENT_AUDIO_STARTED_PLAYING = "EVENT_AUDIO_STARTED_PLAYING";
static constexpr auto EVENT_PLAY_SOUND = "EVENT_PLAY_SOUND";

static constexpr auto EVENT_AUDIO_PLAY_SOUND_CLIP = "EVENT_AUDIO_PLAY_SOUND_CLIP";
static constexpr auto EVENT_AUDIO_STOP_SOUND_CLIP = "EVENT_AUDIO_STOP_SOUND_CLIP";
static constexpr auto EVENT_AUDIO_PLAY_MOVIE_QUOTE = "EVENT_AUDIO_PLAY_MOVIE_QUOTE";
static constexpr auto EVENT_AUDIO_STOP_MOVIE_QUOTE = "EVENT_AUDIO_STOP_MOVIE_QUOTE";

static constexpr auto EVENT_CUSTOM_PACK_STARTUP_1 = "EVENT_CUSTOM_PACK_STARTUP_1";

static constexpr auto EVENT_AUDIO_TOGGLE_BACKGROUND = "EVENT_AUDIO_TOGGLE_BACKGROUND";
static constexpr auto EVENT_AUDIO_SHUFFLE_BACKGROUND = "EVENT_AUDIO_SHUFFLE_BACKGROUND";
static constexpr auto EVENT_AUDIO_RESET_BACKGROUND = "EVENT_AUDIO_RESET_BACKGROUND";
static constexpr auto EVENT_AUDIO_STOP_BACKGROUND = "EVENT_AUDIO_STOP_BACKGROUND";

// Pack events
static constexpr auto EVENT_PACK_BOOTING = "EVENT_PACK_BOOTING";
static constexpr auto EVENT_PACK_IDLE = "EVENT_PACK_IDLE";
static constexpr auto EVENT_PACK_SHUTTING_DOWN = "EVENT_PACK_SHUTTING_DOWN"; // pack shutting down
static constexpr auto EVENT_PACK_OFF = "EVENT_PACK_OFF";  // pack off

static constexpr auto EVENT_THEME_CHANGE = "EVENT_THEME_CHANGE";

// Wand events
static constexpr auto EVENT_WAND_BOOTING = "EVENT_WAND_BOOTING";
static constexpr auto EVENT_WAND_IDLE = "EVENT_WAND_IDLE";
static constexpr auto EVENT_WAND_FIRING_START = "EVENT_WAND_FIRING_START";
static constexpr auto EVENT_WAND_FIRING_STOP = "EVENT_WAND_FIRING_STOP";
static constexpr auto EVENT_WAND_SHUTTING_DOWN = "EVENT_WAND_SHUTTING_DOWN";
static constexpr auto EVENT_WAND_OFF = "EVENT_WAND_OFF";

// Overheat events
static constexpr auto EVENT_OVERHEAT = "EVENT_OVERHEAT";

// Smoke events
static constexpr auto EVENT_SMOKE_ON = "EVENT_SMOKE_ON";
static constexpr auto EVENT_SMOKE_OFF = "EVENT_SMOKE_OFF";
static constexpr auto EVENT_VENT_PACK = "EVENT_VENT_PACK";
static constexpr auto EVENT_OVERHEAT_COMPLETE = "EVENT_OVERHEAT_COMPLETE";

// Events to enable/disable the vent
static constexpr auto EVENT_VENT_WAND_ON = "EVENT_WAND_VENT_ON";
static constexpr auto EVENT_VENT_WAND_OFF = "EVENT_WAND_VENT_OFF";


static constexpr auto EVENT_BLUETOOTH_METADATA = "EVENT_BLUETOOTH_METADATA_CHANGE";
static constexpr auto EVENT_BLUETOOTH_AUDIO_STATE_CHANGE = "EVENT_BLUETOOTH_AUDIO_STATE_CHANGE";
static constexpr auto EVENT_BLUETOOTH_CONNECTION_STATE_CHANGE = "EVENT_BLUETOOTH_CONNECTION_STATE_CHANGE";
static constexpr auto EVENT_BLUETOOTH_VOLUME_CHANGE = "EVENT_BLUETOOTH_VOLUME_CHANGE";
static constexpr auto EVENT_BLUETOOTH_STARTED = "EVENT_BLUETOOTH_STARTED";
static constexpr auto EVENT_BLUETOOTH_STOPPED = "EVENT_BLUETOOTH_STOPPED";
static constexpr auto CMD_START_BLUETOOTH = "CMD_START_BLUETOOTH";
static constexpr auto CMD_STOP_BLUETOOTH = "CMD_STOP_BLUETOOTH";
static constexpr auto CMD_BLUETOOTH_PLAY = "CMD_BLUETOOTH_PLAY";
static constexpr auto CMD_BLUETOOTH_PAUSE = "CMD_BLUETOOTH_PAUSE";


struct EventArgs
{
  String eventName;
  String eventDetail1;
  String eventDetail2;
  String emitFollowUpEvent = "";
};

#endif

