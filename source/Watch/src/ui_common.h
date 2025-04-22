#pragma once

const int MENU_WIDTH=240;
const int MENU_HEIGHT=220; // -20 for status bar
bool screenOn = true;
bool packOn = false;
bool wandOn = false;
bool backgroundAudio = true;
int volume = 100;

// https://learn.adafruit.com/adafruit-drv2605-haptic-controller-breakout/arduino-code
static int HAPTIC_WAVEFORM_CLICK = 17;

lv_obj_t *main_menu;
lv_obj_t *main_page;