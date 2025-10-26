#include "menus/theme.h"
#include "menus/settings.h"
#include "menus/operations.h"
#include "menus/audio.h"
#include <lvgl.h>
#include "../ui_common.h"
#include <LilyGoLib.h>
#include <LV_Helper.h>


lv_obj_t * get_status_bar() {
    // Create a status bar
    lv_obj_t *status_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(status_bar, MENU_WIDTH, 20); // Set the height of the status bar
    lv_obj_align(status_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0x333333), 0); // Set background color
    lv_obj_set_style_text_color(status_bar, lv_color_hex(0xFFFFFF), 0); // Set text color

    // Add a label to display the battery percentage
    lv_obj_t *battery_label = lv_label_create(status_bar);
    lv_label_set_text_fmt(battery_label, "Bat: %d%%", instance.pmu.getBatteryPercent());
    lv_obj_align(battery_label, LV_ALIGN_RIGHT_MID, 0, 0); // Align to the far right with some padding
    return status_bar;
}

void setupHeader(lv_obj_t *main_menu) {

    // Move the header and back button to the bottom of the screen
    lv_obj_t * header = lv_menu_get_main_header(main_menu);
    lv_obj_set_style_pad_top(header, 10, 0);
    lv_obj_t * back_btn = lv_menu_get_main_header_back_button(main_menu);
    lv_obj_t * back_img = lv_obj_get_child(back_btn, 0);
    lv_obj_set_size(back_img, 0, 0);

    lv_obj_t * back_button_label = lv_label_create(back_btn);
    lv_obj_set_style_text_font(back_button_label, menufont, 0);
    lv_label_set_text(back_button_label, "  \xEF\x81\x93\xEF\x81\x93 Back");
    lv_menu_set_mode_header(main_menu, LV_MENU_HEADER_BOTTOM_FIXED);
}


lv_obj_t * create_main_view() {

    lv_obj_t *status_bar = get_status_bar();
    lv_obj_t *btn;

    main_menu = lv_menu_create(lv_scr_act());
    lv_obj_set_size(main_menu, MENU_WIDTH, MENU_HEIGHT);
    lv_obj_align(main_menu, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_font(main_menu,  menufont, LV_PART_MAIN);

    setupHeader(main_menu);

    main_page = lv_menu_page_create(main_menu, NULL);
    lv_list_add_text(main_page, "Main Menu");

    lv_obj_t *operations_page = lv_menu_page_create(main_menu, NULL);
    create_operation_menu(operations_page);
    btn = lv_list_add_btn(main_page, LV_SYMBOL_POWER, "Operations");
    lv_menu_set_load_page_event(main_menu, btn, operations_page);

    lv_obj_t *theme_page = lv_menu_page_create(main_menu, NULL);
    create_theme_menu(theme_page);
    btn = lv_list_add_btn(main_page, LV_SYMBOL_LOOP, "Change Theme");
    lv_menu_set_load_page_event(main_menu, btn, theme_page);

    lv_obj_t *audio_page = lv_menu_page_create(main_menu, NULL);
    create_audio_menu(audio_page);
    btn = lv_list_add_btn(main_page, LV_SYMBOL_VOLUME_MAX, "Audio");
    lv_menu_set_load_page_event(main_menu, btn, audio_page);


    btn = lv_list_add_btn(main_page, LV_SYMBOL_BLUETOOTH, "Bluetooth");
//    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void*)&EVENT_WATCH_TURN_WAND_ON);

    lv_obj_t *settings_page = lv_menu_page_create(main_menu, NULL);
    create_settings_menu(settings_page);
    btn = lv_list_add_btn(main_page, LV_SYMBOL_SETTINGS, "Settings");
    lv_menu_set_load_page_event(main_menu, btn, settings_page);

    lv_menu_set_page(main_menu, main_page);

    return main_menu;
}