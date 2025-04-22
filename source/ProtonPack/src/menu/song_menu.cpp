#include "menu.h"
#include "../common.h"

// this needs some bytes in ram
#define MAX_FILES_IN_LIST 20

// scroll row position
uint8_t scroll_row;
// cursor real position on lcd lines (0 - _LCDML_DISP_rows - 1)
uint8_t cursor_real_pos;
// max filelist count
uint8_t current_max_list_count;
// cursor position to file list entry
uint8_t cursor_position_cur;
static char filelist[MAX_FILES_IN_LIST][21];


// *********************************************************************
void mFunc_songlist(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    debugln("inside mFunc_songlist - setup");
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup
    // is called only if it is started

    // use this to init some vars

    // set max file counter
    current_max_list_count = 0;
    // set current cursor position
    int cursor_position_cur = 0;
    //
    scroll_row = 0;
    cursor_real_pos = 0;

    // for example read sd card files here

    // for example here only with a dummy list
    // with 12 dummyfiles
    // subfolders are not supported

    // generate file names
    current_max_list_count = 3;
    // for(uint8_t i = 0; i<3; i++) {
    //   if(i < MAX_FILES_IN_LIST) {
    //     // copy dummy string to new position
    //     // memcpy(filelist[i], "File   .txt", 11);

    //     if(i<10) {                    // filenames < 10
    //       filelist[i][5] = 0+48;      // number + '0'     '0' = 48
    //       filelist[i][6] = i+48;      // number
    //     } else {                      // filenames >= 10
    //       filelist[i][5] = i/10+48;
    //       filelist[i][6] = i%10+48;
    //     }
    //     current_max_list_count++;
    //   }
    // }

    u8g2.setFont(getFont());
    u8g2.firstPage();
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    debug("l");
    // loop
    // is called when it is triggered
    // - with LCDML_DISP_triggerMenu( milliseconds )
    // - with every button status change

    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // init some vars for scrollbar
    uint8_t n_max             = (current_max_list_count >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (current_max_list_count);;
    uint8_t scrollbar_min     = 0;
    uint8_t scrollbar_max     = current_max_list_count;
    //uint8_t scrollbar_cur_pos = cursor_position_cur;
    uint8_t scroll_pos        = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * cursor_position_cur);

    if(LCDML.BT_checkUp())
    {
      // scroll up
      if(cursor_position_cur > 0) { // check minimum cursor
        cursor_position_cur--;      // scroll up
        if(cursor_real_pos == 0) {
          scroll_row--;             // scroll display rows
        }
      }
      if(cursor_position_cur == 0) { // check minimum cursor
        cursor_position_cur = current_max_list_count-1;
        if(cursor_real_pos == 0) {
          scroll_row++;             // scroll display rows
        }
      }
    }

    if(LCDML.BT_checkDown())
    {
      // scroll down
      if(cursor_position_cur < (current_max_list_count-1)) {    // check list end
        cursor_position_cur++;                                  // go to next element
        if(cursor_real_pos == (_LCDML_DISP_rows-1)) {           // check if current cursor is in last display line
          scroll_row++;                                         // scroll content
        }
      }
    }


    // clear display
    // ================


    // display content
    // ==================
    for (uint8_t n = scroll_row; n < (scroll_row+_LCDML_DISP_rows); n++)
    {
      #ifndef _LCDML_USE_SERIAL_MONITOR
        // set cursor
        // u8g2.setCursor(1, n-scroll_row);
        // // set content
        // u8g2.print(filelist[n]);
        debugln(filelist[n]);
        u8g2.drawStr(0, (getFontHeight() * (n-scroll_row)), filelist[n]);
      #else
        if (n == cursor_position_cur)
        {
          debug(F("->"));
          cursor_real_pos = n-scroll_row;
        }
        else
        {
          debug(F("  "));
        }
        debugln(filelist[n]);
      #endif
    }
    u8g2.nextPage();

    if(LCDML.BT_checkEnter())
    {
      debugln("Selected song");
      debugln(filelist[cursor_position_cur]);
      // audioEngine.playBackground(filelist[cursor_position_cur]);
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // loop end
    // you can here reset some global vars or delete it
  }
}