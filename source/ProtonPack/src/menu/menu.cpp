#include "menu.h"

int getDrawFrame() {
    return _LCDML_DISP_draw_frame;
}

const unsigned char* getFont() {
    return _LCDML_DISP_font;
}
char* getDisplayCursorChar() {
    // return _LCDML_DISP_cursor_char;
    return (char*)">";
}
int getScrollbarWidth() {
    return _LCDML_DISP_scrollbar_w;
}
int getCursorSpaceBehind() {
    return _LCDML_DISP_cur_space_behind;
}
int getCursorSpaceBefore() {
    return _LCDML_DISP_cur_space_before;
}
int getDispBoxX0() {
    return _LCDML_DISP_box_x0;
}
int getDispBoxX1() {
    return _LCDML_DISP_box_x1;
}
int getDispBoxY0() {
    return _LCDML_DISP_box_y0;
}
int getDispBoxY1() {
    return _LCDML_DISP_box_y1;
}

int getFontHeight() {
    return _LCDML_DISP_font_h;
}
int getFontWidth() {
    return _LCDML_DISP_font_w;
}

int getRows() {
    return _LCDML_DISP_rows;
}

int getCols() {
    return _LCDML_DISP_cols;
}
// *********************************************************************
// Prototypes
// *********************************************************************
  void lcdml_menu_display();
  void lcdml_menu_clear();
  void lcdml_menu_control();


// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // LCDML_0        => layer 0
  // LCDML_0_X      => layer 1
  // LCDML_0_X_X    => layer 2
  // LCDML_0_X_X_X  => layer 3
  // LCDML_0_...      => layer ...

  // For beginners
//   // LCDML_add(id, prev_layer, new_num, lang_char_array, callback_function)
//   LCDML_add         (0  , LCDML_0         , 1  , "Information"      , mFunc_information);       // this menu function can be found on "LCDML_display_menuFunction" tab
//   LCDML_add         (1  , LCDML_0         , 2  , "Time info"        , mFunc_timer_info);        // this menu function can be found on "LCDML_display_menuFunction" tab
//   LCDML_add         (2  , LCDML_0         , 3  , "Program"          , NULL);                    // NULL = no menu function
//   LCDML_add         (3  , LCDML_0_3       , 1  , "Program 1"        , NULL);                    // NULL = no menu function
//   LCDML_add         (4  , LCDML_0_3_1     , 1  , "P1 dummy"         , NULL);                    // NULL = no menu function
//   LCDML_add         (5  , LCDML_0_3_1     , 2  , "P1 Settings"      , NULL);                    // NULL = no menu function
//   LCDML_add         (6  , LCDML_0_3_1_2   , 1  , "Warm"             , NULL);                    // NULL = no menu function
//   LCDML_add         (7  , LCDML_0_3_1_2   , 2  , "Cold"             , NULL);                    // NULL = no menu function
//   LCDML_add         (8  , LCDML_0_3_1_2   , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
//   LCDML_add         (9  , LCDML_0_3_1     , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
//   LCDML_add         (10 , LCDML_0_3       , 2  , "Program 2"        , mFunc_p2);                // this menu function can be found on "LCDML_display_menuFunction" tab
//   LCDML_add         (11 , LCDML_0_3       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab

//   LCDML_add         (12 , LCDML_0         , 4  , "Special"          , NULL);                    // NULL = no menu function
//   LCDML_add         (13 , LCDML_0_4       , 1  , "Go to Root"       , mFunc_goToRootMenu);      // this menu function can be found on "LCDML_display_menuFunction" tab
//   LCDML_add         (14 , LCDML_0_4       , 2  , "Jump to Time info", mFunc_jumpTo_timer_info); // this menu function can be found on "LCDML_display_menuFunction" tab
//   LCDML_add         (15 , LCDML_0_4       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab


//   // Advanced menu (for profit) part with more settings
//   // Example for one function and different parameters
//   // It is recommend to use parameters for switching settings like, (small drink, medium drink, big drink) or (200ml, 400ml, 600ml, 800ml) ...
//   // the parameter change can also be released with dynParams on the next example
//   // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
//   LCDML_addAdvanced (16 , LCDML_0         , 5  , NULL,          "Parameter"      , NULL,                0,            _LCDML_TYPE_default);                    // NULL = no menu function
//   LCDML_addAdvanced (17 , LCDML_0_5       , 1  , NULL,          "Parameter 1"      , mFunc_para,       10,            _LCDML_TYPE_default);                    // NULL = no menu function
//   LCDML_addAdvanced (18 , LCDML_0_5       , 2  , NULL,          "Parameter 2"      , mFunc_para,       20,            _LCDML_TYPE_default);                    // NULL = no menu function
//   LCDML_addAdvanced (19 , LCDML_0_5       , 3  , NULL,          "Parameter 3"      , mFunc_para,       30,            _LCDML_TYPE_default);                    // NULL = no menu function
//   LCDML_add         (20 , LCDML_0_5       , 4  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab


//   // Example for dynamic content
//   // 1. set the string to ""
//   // 2. use type  _LCDML_TYPE_dynParam   instead of    _LCDML_TYPE_default
//   // this function type can not be used in combination with different parameters
//   // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
//   LCDML_addAdvanced (21 , LCDML_0         , 6  , NULL,          ""                  , mDyn_para,                0,   _LCDML_TYPE_dynParam);                     // NULL = no menu function

//   // Example for conditions (for example for a screensaver)
//   // 1. define a condition as a function of a boolean type -> return false = not displayed, return true = displayed
//   // 2. set the function name as callback (remove the braces '()' it gives bad errors)
//   // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
//   LCDML_addAdvanced (22 , LCDML_0         , 7  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);       // this menu function can be found on "LCDML_display_menuFunction" tab
//   LCDML_addAdvanced (23 , LCDML_0         , 8  , NULL,          ""                  , mBluetooth_para,                0,   _LCDML_TYPE_dynParam);
//   LCDML_addAdvanced (24 , LCDML_0         , 9  , NULL,  "Play random?"               , mPlay_para, 0, _LCDML_TYPE_dynParam);

//   // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

//   // menu element count - last element id
//   // this value must be the same as the last menu element
//   #define _LCDML_DISP_cnt    24

  // create menu




//LCDML_add         (0 , LCDML_0         , 1  , "Songs"         , mFunc_songlist);          // Add song list which can be scrolled
LCDML_addAdvanced (0 , LCDML_0         , 1  , NULL,          "Change Theme"      , NULL,             0,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (1 , LCDML_0_1       , 1  , NULL,          "1984"      , mFunc_changeTheme,       0,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (2 , LCDML_0_1       , 2  , NULL,          "Afterlife"      , mFunc_changeTheme,       1,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (3 , LCDML_0_1       , 3  , NULL,          "Slime"      , mFunc_changeTheme,       2,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (4 , LCDML_0_1       , 4  , NULL,          "Mason"      , mFunc_changeTheme,       3,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (5 , LCDML_0_1       , 5  , NULL,          "Christmas"      , mFunc_changeTheme,       4,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (6 , LCDML_0_1       , 6  , NULL,          "KITT"      , mFunc_changeTheme,       5,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (7 , LCDML_0_1       , 7  , NULL,          "Hannukah"      , mFunc_changeTheme,       6,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (8 , LCDML_0_1       , 8  , NULL,          "St. Patrick"      , mFunc_changeTheme,       7,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (9 , LCDML_0_1       , 9  , NULL,          "Baseball"      , mFunc_changeTheme,       8,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_add         (10 , LCDML_0_1       , 10  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
LCDML_addAdvanced (11 , LCDML_0         , 2  , NULL,          ""                  , mHumming_para,                0,   _LCDML_TYPE_dynParam);

LCDML_addAdvanced (12 , LCDML_0         , 3  , NULL,          "Special"      , NULL,             0,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (13 , LCDML_0_3       , 1  , NULL,          "Pack Startup"      , mFunc_packStart,  6,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (14 , LCDML_0_3       , 2  , NULL,          "Vent - Short"      , mFunc_vent,       6,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_addAdvanced (15 , LCDML_0_3       , 3  , NULL,          "Vent - Long"      , mFunc_vent,       12,            _LCDML_TYPE_default);                    // NULL = no menu function
LCDML_add         (16 , LCDML_0_3       , 4  , "Back"             , mFunc_back);


LCDML_addAdvanced (17 , LCDML_0         , 4  , NULL,          ""                  , mVolume_para,                0,   _LCDML_TYPE_dynParam);                     // NULL = no menu function)
LCDML_addAdvanced (18, LCDML_0         , 5  , NULL,          ""                  , mBluetooth_para,                0,   _LCDML_TYPE_dynParam);
LCDML_addAdvanced (19, LCDML_0         , 6  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);       // this menu function can be found on "LCDML_display_menuFunction" tab


#define _LCDML_DISP_cnt    19
LCDML_createMenu(_LCDML_DISP_cnt);

void setupMenu() {
    LCDML_setup(_LCDML_DISP_cnt);
    // Enable Menu Rollover
    LCDML.MENU_enRollover();
}
