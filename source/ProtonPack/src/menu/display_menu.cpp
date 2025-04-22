#define FASTLED_INTERNAL // remove annoying pragma messages
// =====================================================================
//
// Output function
//
// =====================================================================


/* ******************************************************************** */
#include "menu.h"
void lcdml_menu_clear()
/* ******************************************************************** */
{
}

/* ******************************************************************** */
void lcdml_menu_display()
/* ******************************************************************** */
{
  // for first test set font here
  u8g2.setFont(getFont());

  // declaration of some variables
  // ***************
  // content variable
  char content_text[getCols()];  // save the content text of every menu element
  // menu element object
  LCDMenuLib2_menu *tmp;
  // some limit values
  uint8_t i = LCDML.MENU_getScroll();
  uint8_t maxi = getRows() + i;
  uint8_t n = 0;

   // init vars
  uint8_t n_max             = (LCDML.MENU_getChilds() >= getRows()) ? getRows() : (LCDML.MENU_getChilds());

  uint8_t scrollbar_min     = 0;
  uint8_t scrollbar_max     = LCDML.MENU_getChilds();
  uint8_t scrollbar_cur_pos = LCDML.MENU_getCursorPosAbs();
  uint8_t scroll_pos        = ((1.*n_max * getRows()) / (scrollbar_max - 1) * scrollbar_cur_pos);

  // generate content
  u8g2.firstPage();
  do {


    n = 0;
    i = LCDML.MENU_getScroll();
    // update content
    // ***************

      // clear menu
      // ***************

    // check if this element has children
    if ((tmp = LCDML.MENU_getDisplayedObj()) != NULL)
    {
      // loop to display lines
      do
      {
        // check if a menu element has a condition and if the condition be true
        if (tmp->checkCondition())
        {
          // check the type off a menu element
          if(tmp->checkType_menu() == true)
          {
            // display normal content
            LCDML_getContent(content_text, tmp->getID());
            u8g2.drawStr( getDispBoxX0() +getFontWidth() + getCursorSpaceBehind(), getDispBoxY0() + getFontHeight() * (n + 1), content_text);
          }
          else
          {
            if(tmp->checkType_dynParam()) {
              tmp->callback(n);
            }
          }
          // increment some values
          i++;
          n++;
        }
      // try to go to the next sibling and check the number of displayed rows
      } while (((tmp = tmp->getSibling(1)) != NULL) && (i < maxi));
    }

    // set cursor
    u8g2.drawStr( getDispBoxX0()+getCursorSpaceBefore(), getDispBoxY0() + getFontHeight() * (LCDML.MENU_getCursorPos() + 1),  getDisplayCursorChar());

    if(getDrawFrame() == 1) {
       u8g2.drawFrame(getDispBoxX0(), getDispBoxY0(), (getDispBoxX1()-getDispBoxX0()), (getDispBoxY1()-getDispBoxY0()));
    }

    // display scrollbar when more content as rows available and with > 2
    if (scrollbar_max > n_max && getScrollbarWidth() > 2)
    {
      // set frame for scrollbar
      u8g2.drawFrame(getDispBoxX1() - getScrollbarWidth(), getDispBoxY0(), getScrollbarWidth(), getDispBoxY1()-getDispBoxY0());

      // calculate scrollbar length
      uint8_t scrollbar_block_length = scrollbar_max - n_max;
      scrollbar_block_length = (getDispBoxY1()-getDispBoxY0()) / (scrollbar_block_length + getRows());

      //set scrollbar
      if (scrollbar_cur_pos == 0) {                                   // top position     (min)
        u8g2.drawBox(getDispBoxX1() - (getScrollbarWidth()-1), getDispBoxY0() + 1                                                     , (getScrollbarWidth()-2)  , scrollbar_block_length);
      }
      else if (scrollbar_cur_pos == (scrollbar_max-1)) {            // bottom position  (max)
        u8g2.drawBox(getDispBoxX1() - (getScrollbarWidth()-1), getDispBoxY1() - scrollbar_block_length                                , (getScrollbarWidth()-2)  , scrollbar_block_length);
      }
      else {                                                                // between top and bottom
        u8g2.drawBox(getDispBoxX1() - (getScrollbarWidth()-1), getDispBoxY0() + (scrollbar_block_length * scrollbar_cur_pos + 1),(getScrollbarWidth()-2)  , scrollbar_block_length);
      }
    }
  } while ( u8g2.nextPage() );
}