#include <Arduino.h>
#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <SensorLib.h>
#include "EventArgs.h"
#include "images/gblogo.h"
#include <millisDelay.h>
#include "espnow/EspNowEngine.h"
#include <lvgl.h>
#include "common.h"
#include "ui_common.h"
#include "menus/main.h"
#include "packs.h"


static long SCREEN_TIMEOUT = 30000; // 30 seconds
static long WATCH_SLEEP_TIMEOUT = 120000; // 2 minutes
LV_IMG_DECLARE(gblogo);

millisDelay ms_logo;
millisDelay ms_screenTimeout;
millisDelay ms_checkLastTouch;
millisDelay ms_watchSleepTimeout;
EspNowEngine espNowEngine;

RTC_DATA_ATTR int bootCount = 0;
bool  pmu_flag = false;

const char *get_wakeup_reason()
{
    switch (esp_sleep_get_wakeup_cause()) {
    case ESP_SLEEP_WAKEUP_EXT0 : return ("Wakeup caused by external signal using RTC_IO");
    case ESP_SLEEP_WAKEUP_EXT1 : return ("Wakeup caused by external signal using RTC_CNTL");
    case ESP_SLEEP_WAKEUP_TIMER : return ("Wakeup caused by timer");
    case ESP_SLEEP_WAKEUP_TOUCHPAD : return ("Wakeup caused by touchpad");
    case ESP_SLEEP_WAKEUP_ULP : return ("Wakeup caused by ULP program");
    default : return ("Wakeup was not caused");
    }
}

void setFlag(void)
{
    pmu_flag = true;
}



// todo
// operation menu ( vent)
// change volume
// cycle background , turn on/off background
// trigger custom sound
// simple menu (theme, background, volume, special, bluetooth)



lv_obj_t *tv;
const lv_font_t *menufont = &lv_font_montserrat_20;

void initEspNow() {

    espNowEngine.init();
    espNowEngine.setPeerAddress(packs[packSelection]);

}

void savePrefs() {
    Preferences prefs = getPrefs();
    prefs.putInt("pack", packSelection);
    closePrefs();
}

void loadPrefs() {
    Preferences prefs = getPrefs();
    packSelection = prefs.getInt("pack", 0);
    if (packSelection > sizeof(packs) / sizeof(packs[0])) {
        packSelection = 0;
    }
    closePrefs();
}

void showLogo() {
    watch.clearScreen();
    lv_obj_t * bg = lv_image_create(lv_screen_active());
    lv_image_set_src(bg, &gblogo);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(bg, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), 0);

    ms_logo.start(1200);

}

void handleStart() {
    bootCount++;
    if (bootCount < 2) {
        // only show the boot logo if this is a cold boot
        showLogo();
    } else {
        watch.clearScreen();
        create_main_view();
    }
}

void setup() {
    Serial.begin(115200);
    watch.begin();
    beginLvglHelper();
    loadPrefs();

    initEspNow();
    ms_screenTimeout.start(SCREEN_TIMEOUT); // 30 seconds screen timeout
    ms_checkLastTouch.start(500);
    ms_watchSleepTimeout.start(WATCH_SLEEP_TIMEOUT); // 2 minutes watch sleep timeout
    watch.clearPMU();
    handleStart();
  }


void processTimers() {

    if (ms_logo.justFinished()) {
        watch.clearScreen();
        create_main_view();
      }
      if (ms_screenTimeout.justFinished()) {
        watch.setBrightness(0); // turn off the screen
        screenOn = false;
      }
      if (ms_checkLastTouch.justFinished()) {
        ms_checkLastTouch.restart();
        if (millis() - watch.getLastTouch() < 300) {
            // touched
            ms_screenTimeout.restart();
            ms_watchSleepTimeout.restart();
            if (!screenOn) {
                watch.setBrightness(50); // turn on the screen
                screenOn = true;
                lv_tick_inc(LV_DISP_DEF_REFR_PERIOD);
                ms_watchSleepTimeout.restart();
            }
        }
      }
      if (ms_watchSleepTimeout.justFinished()) {
        // put watch to sleep
        watch.setSleepMode(TOUCH_WAKEUP);
        watch.sleep();
      }
}

void loop() {
    delay(5);
    lv_task_handler();
    lv_tick_inc(5);
    processTimers();
}


