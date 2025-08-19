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


static long SCREEN_TIMEOUT = 20000; // 20 seconds
static long WATCH_SLEEP_TIMEOUT = 120000; // 2 minutes
LV_IMG_DECLARE(gblogo);

millisDelay ms_logo;
millisDelay ms_screenTimeout;
millisDelay ms_checkLastTouch;
millisDelay ms_watchSleepTimeout;
EspNowEngine espNowEngine;

RTC_DATA_ATTR int bootCount = 0;
bool  pmu_flag = false;

long lastTouch = 0;

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

static void touchpad_read( lv_indev_t *drv, lv_indev_data_t *data )
{
    static int16_t x, y;
    auto *plane = (LilyGo_Display *)lv_indev_get_user_data(drv);
    uint8_t touched = plane->getPoint(&x, &y, 1);
    if ( touched ) {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
        lastTouch = millis();
        return;
    }
    data->state = LV_INDEV_STATE_REL;
}

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

void clearScreen() {
  lv_obj_clean(lv_screen_active());
}

void showLogo() {
    clearScreen();
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
        clearScreen();
        create_main_view();
    }
}

void setup() {
    Serial.begin(115200);
    instance.begin();
    beginLvglHelper(instance);
    lv_indev_t * indev_touch = lv_indev_get_next(NULL);
    lv_indev_set_type(indev_touch, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touch, touchpad_read);
    instance.setBrightness(50);

    loadPrefs();

    initEspNow();
    ms_screenTimeout.start(SCREEN_TIMEOUT); // 30 seconds screen timeout
    ms_checkLastTouch.start(500);
    ms_watchSleepTimeout.start(WATCH_SLEEP_TIMEOUT); // 2 minutes watch sleep timeout

    handleStart();

    instance.onEvent([](DeviceEvent_t event, void * user_data) {
        ms_watchSleepTimeout.restart();
        espNowEngine.sendEvent(EVENT_AUDIO_SHUFFLE_BACKGROUND);
    }, PMU_EVENT_KEY_CLICKED, NULL);

    instance.onEvent([](DeviceEvent_t event, void * user_data) {
        ms_watchSleepTimeout.restart();
        espNowEngine.sendEventWithDetail("EVENT_SMOKE_TURN_ON", "5000");
    }, PMU_EVENT_KEY_LONG_PRESSED, NULL);

  }


void processTimers() {

    if (ms_logo.justFinished()) {
        clearScreen();
        create_main_view();
      }
      if (ms_screenTimeout.justFinished()) {
        instance.setBrightness(0); // turn off the screen
        screenOn = false;
        debugln("screen off");
        // instance.lightSleep(WAKEUP_SRC_TOUCH_PANEL);
        // ms_screenTimeout.restart();
      }
      if (ms_checkLastTouch.justFinished()) {
        ms_checkLastTouch.restart();
        if (millis() - lastTouch < 300) {
            // touched
            ms_screenTimeout.restart();
            ms_watchSleepTimeout.restart();
            if (!screenOn) {
                instance.setBrightness(50); // turn on the screen
                screenOn = true;
                debugln("screen on?");
                lv_tick_inc(10);
                ms_watchSleepTimeout.restart();
            }
        }
      }
      if (ms_watchSleepTimeout.justFinished()) {
        // put watch to sleep
        // instance.setSleepMode(TOUCH_WAKEUP);
        instance.sleep(WAKEUP_SRC_TOUCH_PANEL); //WAKEUP_SRC_TOUCH_PANEL);
      }
}


void loop() {
    delay(5);
    lv_task_handler();
    lv_tick_inc(5);
    processTimers();
    instance.loop();
}


