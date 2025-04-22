#pragma once

#include "animations/stream.h"

String VERSION="0.1b";


// display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// menu
LCDMenuLib2_menu LCDML_0(255, 0, 0, NULL, NULL); // root menu element (do not change)
LCDMenuLib2 LCDML(LCDML_0, getRows(), getCols(), lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

bool initFailed = false;
