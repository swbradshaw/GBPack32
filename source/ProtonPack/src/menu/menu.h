#pragma once

#include <LCDMenuLib2.h>
#include <stdint.h>
#include "U8g2lib.h"
#include "../audio/AudioEngine.h"
#include "../EventEngine.h"

  // settings for u8g lib and LCD
  #define _LCDML_DISP_w                 128            // LCD width
  #define _LCDML_DISP_h                 64             // LCD height
  // font settings
  #define _LCDML_DISP_font              u8g_font_6x10  // u8glib font (more fonts under u8g.h line 1520 ...)
  #define _LCDML_DISP_font_w            6              // font width
  #define _LCDML_DISP_font_h            10             // font height
  // cursor settings
  #define _LCDML_DISP_cursor_char       ">"            // cursor char
  #define _LCDML_DISP_cur_space_before  2              // cursor space between
  #define _LCDML_DISP_cur_space_behind  4              // cursor space between
  // menu position and size
  #define _LCDML_DISP_box_x0            0              // start point (x0, y0)
  #define _LCDML_DISP_box_y0            0              // start point (x0, y0)
  #define _LCDML_DISP_box_x1            128            // width x  (x0 + width)
  #define _LCDML_DISP_box_y1            64             // hight y  (y0 + height)
  #define _LCDML_DISP_draw_frame        1              // draw a box around the menu
   // scrollbar width
  #define _LCDML_DISP_scrollbar_w       6  // scrollbar width (if this value is < 3, the scrollbar is disabled)

  // nothing change here
  #define _LCDML_DISP_cols_max          ((_LCDML_DISP_box_x1-_LCDML_DISP_box_x0)/_LCDML_DISP_font_w)
  #define _LCDML_DISP_rows_max          ((_LCDML_DISP_box_y1-_LCDML_DISP_box_y0-((_LCDML_DISP_box_y1-_LCDML_DISP_box_y0)/_LCDML_DISP_font_h))/_LCDML_DISP_font_h)

  // rows and cols
  // when you use more rows or cols as allowed change in LCDMenuLib.h the define "_LCDML_DISP_cfg_max_rows" and "_LCDML_DISP_cfg_max_string_length"
  // the program needs more ram with this changes
  #define _LCDML_DISP_rows              _LCDML_DISP_rows_max  // max rows
  #define _LCDML_DISP_cols              20                   // max cols

  extern char* g_LCDML_DISP_lang_lcdml_table[254];

const unsigned char* getFont();
char* getDisplayCursorChar();
void mFunc_information(uint8_t param);
void mFunc_timer_info(uint8_t param);
void mFunc_back(uint8_t param);
void mFunc_p2(uint8_t param);
void mFunc_para(uint8_t param);
void mVolume_para(uint8_t param);
// void mDyn_para(uint8_t param);
void mPlay_para(uint8_t param);
void mFunc_jumpTo_timer_info(uint8_t param);
void mFunc_goToRootMenu(uint8_t param);
void mBluetooth_para(uint8_t line);
void mFunc_songlist(uint8_t line);
void mHumming_para(uint8_t line);
void mFunc_screensaver(uint8_t param);
void mFunc_changeTheme(uint8_t param);
void mFunc_vent(uint8_t param);
void mFunc_packStart(uint8_t line);
bool COND_hide();
int getDrawFrame();
int getScrollbarWidth();
int getCursorSpaceBehind();
int getCursorSpaceBefore();
int getDispBoxX0();
int getDispBoxX1();
int getDispBoxY0();
int getDispBoxY1();
int getFontHeight();
int getFontWidth();
int getRows();
int getCols();
void setupMenu();
void lcdml_menu_display();
void lcdml_menu_clear();
void lcdml_menu_control();

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

extern LCDMenuLib2_menu LCDML_0;
extern LCDMenuLib2 LCDML;

extern EventEngine eventEngine;
extern AudioEngine audioEngine;
