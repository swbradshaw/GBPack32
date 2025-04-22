#include "../espnow/EspNowEngine.h"
#include <lvgl.h>
#include "../ui_common.h"
#include "EventArgs.h"
#include "../packs.h"

extern EspNowEngine espNowEngine;
extern const lv_font_t *menufont;
extern void initEspNow();
extern void savePrefs();

static lv_style_t style_radio;
static lv_style_t style_radio_chk;

static void radiobutton_create(lv_obj_t * parent, const char * txt)
{
    lv_obj_t * obj = lv_checkbox_create(parent);
    lv_checkbox_set_text(obj, txt);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_style(obj, &style_radio, LV_PART_INDICATOR);
    lv_obj_add_style(obj, &style_radio_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);
}

static void pack_selection_event_handler(lv_event_t * e)
{
    lv_obj_t * cont = (lv_obj_t *)lv_event_get_current_target(e);
    lv_obj_t * act_cb = lv_event_get_target_obj(e);
    lv_obj_t * old_cb = lv_obj_get_child(cont, packSelection);

    /*Do nothing if the container was clicked*/
    if(act_cb == cont) return;

    lv_obj_remove_state(old_cb, LV_STATE_CHECKED);   /*Uncheck the previous radio button*/
    lv_obj_add_state(act_cb, LV_STATE_CHECKED);     /*Check the current radio button*/

    packSelection = lv_obj_get_index(act_cb);
    espNowEngine.deinit();
    initEspNow();
    savePrefs();

}

lv_obj_t * create_settings_menu(lv_obj_t * parent) {
    lv_obj_t *settings_menu = lv_list_create(parent);

    lv_obj_remove_flag(settings_menu, LV_OBJ_FLAG_SCROLL_ELASTIC); // Remove scroll one flag
    lv_obj_remove_flag(settings_menu, LV_OBJ_FLAG_SCROLLABLE); // Remove scrollable flag

    lv_obj_set_size(settings_menu, MENU_WIDTH, MENU_HEIGHT);
    lv_obj_center(settings_menu);

    lv_obj_set_style_text_font(settings_menu,  menufont, LV_PART_MAIN);

    lv_obj_t *btn;
    lv_list_add_text(settings_menu, "Settings");

    lv_style_init(&style_radio);
    lv_style_set_radius(&style_radio, LV_RADIUS_CIRCLE);

    lv_style_init(&style_radio_chk);
    lv_style_set_bg_image_src(&style_radio_chk, NULL);


    lv_obj_t * packSelectionView = lv_obj_create(settings_menu);
    lv_obj_set_flex_flow(packSelectionView, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(packSelectionView, MENU_WIDTH, 100);
    lv_obj_add_event_cb(packSelectionView, pack_selection_event_handler, LV_EVENT_CLICKED, &packSelection);
    uint32_t i;
    char buf[32];

    for(i = 0; i < sizeof(packs) / sizeof(packs[0]); i++) {
        lv_snprintf(buf, sizeof(buf), "Pack %02X%02X", packs[i][4], packs[i][5]);
        radiobutton_create(packSelectionView, buf);
    }
    lv_obj_add_state(lv_obj_get_child(packSelectionView, packSelection), LV_STATE_CHECKED);


    return settings_menu;
}
