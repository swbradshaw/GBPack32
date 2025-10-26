#include "../espnow/EspNowEngine.h"
#include <lvgl.h>
#include "../ui_common.h"

extern EspNowEngine espNowEngine;
extern const lv_font_t *menufont;

static void theme_event_handler(lv_event_t *e)
{
    if (screenOn) {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
        if (code == LV_EVENT_CLICKED)
        {
            // Serial.printf("Clicked: %s\n", lv_list_get_btn_text(theme_menu, obj));
            Serial.printf("Event: %s\n", lv_event_get_user_data(e));
            espNowEngine.sendEventWithDetail("EVENT_THEME_CHANGE", (const char*)lv_event_get_user_data(e));
            lv_menu_clear_history(main_menu);
            lv_menu_set_page(main_menu, main_page);
        }
    }
}
lv_obj_t * create_theme_menu(lv_obj_t * parent) {
    lv_obj_t *theme_menu = lv_list_create(parent);

    lv_obj_set_size(theme_menu, MENU_WIDTH, MENU_HEIGHT-35);

    lv_obj_t *btn;
    lv_list_add_text(theme_menu, "Change Theme");
    btn = lv_list_add_btn(theme_menu, NULL, "1984");
    lv_obj_add_event_cb(btn, theme_event_handler, LV_EVENT_CLICKED, (void*)"0");
    btn = lv_list_add_btn(theme_menu, NULL, "Afterlife");
    lv_obj_add_event_cb(btn, theme_event_handler, LV_EVENT_CLICKED, (void*)"1");
    btn = lv_list_add_btn(theme_menu, NULL, "Slime");
    lv_obj_add_event_cb(btn, theme_event_handler, LV_EVENT_CLICKED, (void*)"2");
    btn = lv_list_add_btn(theme_menu, NULL, "KITT");
    lv_obj_add_event_cb(btn, theme_event_handler, LV_EVENT_CLICKED, (void*)"5");
    btn = lv_list_add_btn(theme_menu, NULL, "Hanukkah");
    lv_obj_add_event_cb(btn, theme_event_handler, LV_EVENT_CLICKED, (void*)"6");
    btn = lv_list_add_btn(theme_menu, NULL, "Irish");
    lv_obj_add_event_cb(btn, theme_event_handler, LV_EVENT_CLICKED, (void*)"7");
    btn = lv_list_add_btn(theme_menu, NULL, "Baseball");
    lv_obj_add_event_cb(btn, theme_event_handler, LV_EVENT_CLICKED, (void*)"8");

    return theme_menu;
}