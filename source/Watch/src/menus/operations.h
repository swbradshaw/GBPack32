#include "../espnow/EspNowEngine.h"
#include <lvgl.h>
#include "../ui_common.h"
#include "EventArgs.h"
#include "LilyGoLib.h"

extern EspNowEngine espNowEngine;
extern const lv_font_t *menufont;
extern LilyGoLib watch;

void handleEvent(const char *event) {
    // Handle the event here
    Serial.printf("Event received: %d\n", event);
    if (strcmp(event, EVENT_WATCH_TURN_PACK_ON) == 0) {
        espNowEngine.sendEvent("EVENT_WATCH_TURN_PACK_ON");
    } else if (strcmp(event, EVENT_VENT_PACK) == 0) {
        espNowEngine.sendEvent("EVENT_VENT_PACK");
    } else if (strcmp(event, EVENT_CUSTOM_PACK_STARTUP_1) == 0) {
        espNowEngine.sendEvent("EVENT_CUSTOM_PACK_STARTUP_1");
    } else if (strcmp(event, EVENT_WATCH_TURN_WAND_ON) == 0) {
        espNowEngine.sendEvent("EVENT_WATCH_TURN_WAND_ON");
    } else if (strcmp(event, EVENT_WATCH_TURN_WAND_OFF) == 0) {
        espNowEngine.sendEvent("EVENT_WATCH_TURN_WAND_OFF");
    }
    watch.setWaveform(0, HAPTIC_WAVEFORM_CLICK);  // play effect
    watch.run();
}



static void event_handler(lv_event_t *e)
{
    if (screenOn) {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
        if (code == LV_EVENT_CLICKED)
        {
            LV_LOG_USER("Clicked: %s", lv_list_get_btn_text(tv, obj));
            LV_LOG_USER("Event: %s", *(const char **)lv_event_get_user_data(e));
            handleEvent(*(const char **)lv_event_get_user_data(e));
        }
    }
}

lv_obj_t * create_operation_menu(lv_obj_t * parent) {
    lv_obj_t *operation_menu = lv_list_create(parent);

    lv_obj_set_size(operation_menu, MENU_WIDTH, MENU_HEIGHT-35);

    lv_obj_t *btn;
    lv_list_add_text(operation_menu, "Operations");
    btn = lv_list_add_btn(operation_menu, LV_SYMBOL_POWER, "Pack on/off");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)&EVENT_WATCH_TURN_PACK_ON);
    // btn = lv_list_add_btn(operation_menu, LV_SYMBOL_POWER, "Pack off");
    // lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)&EVENT_WATCH_TURN_PACK_OFF);
    btn = lv_list_add_btn(operation_menu, LV_SYMBOL_POWER, "Wand on/off");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)&EVENT_WATCH_TURN_WAND_ON);
    // btn = lv_list_add_btn(operation_menu, LV_SYMBOL_POWER, "Wand off");
    // lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)&EVENT_WATCH_TURN_WAND_OFF);
    btn = lv_list_add_btn(operation_menu, LV_SYMBOL_POWER, "Custom Start");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)&EVENT_CUSTOM_PACK_STARTUP_1);
    btn = lv_list_add_btn(operation_menu, LV_SYMBOL_POWER, "Vent Pack");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)&EVENT_VENT_PACK);

    return operation_menu;
}

