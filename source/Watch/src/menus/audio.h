#include "../espnow/EspNowEngine.h"
#include <lvgl.h>
#include "../ui_common.h"
#include "EventArgs.h"
#include "LilyGoLib.h"

extern EspNowEngine espNowEngine;
extern const lv_font_t *menufont;
lv_obj_t *volume_label;

// static const char * btnm_map[] = {   LV_SYMBOL_AUDIO,     LV_SYMBOL_NEXT,      LV_SYMBOL_STOP  };
static const char * btnm_map[] = {LV_SYMBOL_AUDIO, LV_SYMBOL_NEXT, LV_SYMBOL_STOP, ""   };

lv_obj_t * create_audio_menu(lv_obj_t * parent) {

    lv_obj_t * audio_menu = lv_obj_create(parent);

    lv_obj_set_size(audio_menu, MENU_WIDTH, MENU_HEIGHT);
    // lv_obj_set_scrollbar_mode(audio_menu, LV_SCROLLBAR_MODE_OFF); // Disable scrollbars
    lv_obj_remove_flag(audio_menu, LV_OBJ_FLAG_SCROLLABLE); // Remove scrollable flag
    //lv_obj_remove_flag(audio_menu, LV_OBJ_FLAG_SCROLL_CHAIN); // Remove scroll chain flag
    lv_obj_remove_flag(audio_menu, LV_OBJ_FLAG_SCROLL_ELASTIC); // Remove scroll one flag

    lv_obj_set_style_text_font(audio_menu,  menufont, LV_PART_MAIN);

    lv_obj_t *btn;
    lv_obj_t *title = lv_list_add_text(audio_menu, "Audio");
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, -10); // Align to the top left with some padding

    // add label and slider widget for volume
    volume_label = lv_label_create(audio_menu);
    lv_label_set_text_fmt(volume_label, "Volume: %d", volume);
    lv_obj_align(volume_label, LV_ALIGN_TOP_LEFT, 10, 20); // Align to the top left with some padding
    lv_obj_set_style_text_font(volume_label, menufont, 0);
    // lv_obj_set_style_text_color(volume_label, lv_color_hex(0xFFFFFF), 0); // Set text color

    // add slider widget to control volume
    lv_obj_t *volume_slider = lv_slider_create(audio_menu);
    lv_obj_set_width(volume_slider, MENU_WIDTH-60); // Set width of the slider
    lv_obj_align(volume_slider, LV_ALIGN_TOP_LEFT, 10, 50); // Align below the label
    // lv_obj_align_to(volume_label, volume_slider, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/
    lv_slider_set_range(volume_slider, 0, 100); // Set range of the slider
    lv_slider_set_value(volume_slider, 100, LV_ANIM_OFF); // Set initial value to 50
    lv_obj_add_event_cb(volume_slider, [](lv_event_t *e) {
        if (screenOn) {
            lv_obj_t *volume_slider = lv_event_get_target_obj(e);
            volume = lv_slider_get_value(volume_slider);
            Serial.printf("Volume changed to: %d\n", volume);
            lv_label_set_text_fmt(volume_label, "Volume: %d", volume); // Update label with new volume value
            espNowEngine.sendEventWithDetail("EVENT_AUDIO_VOLUME_CHANGE", String(volume).c_str());

            instance.drv.setWaveform(0, HAPTIC_WAVEFORM_CLICK);  // play effect
            instance.drv.run();
        }
    }, LV_EVENT_RELEASED, NULL);

    lv_obj_t * label = lv_label_create(audio_menu);
    lv_label_set_text(label, "Background Audio");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 70); // Align to the top left with some padding
    lv_obj_set_style_text_font(label, menufont, 0);

     lv_obj_t * btnm1 = lv_buttonmatrix_create(audio_menu);
     lv_buttonmatrix_set_map(btnm1, btnm_map);
     lv_obj_set_height(btnm1, 80);
     lv_obj_set_width(btnm1, MENU_WIDTH-20);
     lv_obj_align(btnm1, LV_ALIGN_BOTTOM_MID, 0, -10); // Align to the top left with some padding
    lv_buttonmatrix_set_button_ctrl(btnm1, 0, LV_BUTTONMATRIX_CTRL_CHECKABLE);
    if (backgroundAudio) {
        lv_buttonmatrix_set_button_ctrl(btnm1, 0, LV_BUTTONMATRIX_CTRL_CHECKED);
    }
    // lv_obj_align(btnm1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btnm1,  [](lv_event_t *e) {
        if (screenOn) {
            lv_event_code_t code = lv_event_get_code(e);
            lv_obj_t * obj = lv_event_get_target_obj(e);
            if(code == LV_EVENT_VALUE_CHANGED) {
                uint32_t id = lv_buttonmatrix_get_selected_button(obj);
                const char * txt = lv_buttonmatrix_get_button_text(obj, id);

                Serial.printf("Button %s was pressed\n", txt);
                LV_UNUSED(txt);
                LV_LOG_USER("%s was pressed\n", txt);

                if (strcmp(txt, LV_SYMBOL_AUDIO) == 0) {
                    espNowEngine.sendEvent(EVENT_AUDIO_TOGGLE_BACKGROUND);
                    backgroundAudio = !backgroundAudio;
                } else if (strcmp(txt, LV_SYMBOL_NEXT) == 0) {
                    espNowEngine.sendEvent(EVENT_AUDIO_SHUFFLE_BACKGROUND);
                } else if (strcmp(txt, LV_SYMBOL_STOP) == 0) {
                    espNowEngine.sendEventWithDetail(EVENT_BUTTON_AUDIO, "LONG_CLICK");
                }

                instance.drv.setWaveform(0, HAPTIC_WAVEFORM_CLICK);  // play effect
                instance.drv.run();
            }
        }
    }, LV_EVENT_ALL, NULL);

    return audio_menu;
}
