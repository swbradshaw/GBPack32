
#include "menu.h"
// #include "../audio/player.h"
void mPlay_para(uint8_t line)
// *********************************************************************
{
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos())
  {
    // make only an action when the cursor stands on this menu item
    //check Button
    if(LCDML.BT_checkAny())
    {
      if(LCDML.BT_checkEnter())
      {
        // playFile("/00/013 protongun_amb_hum_loop.mp3");
        
        LCDML.BT_resetEnter();
      }
    }
  }

  // char buf[20];
  // sprintf (buf, "Bluetooth: %s", isBluetoothOn() ? "ON" : "OFF");

  // // setup function
  // u8g2.drawStr( getDispBoxX0() + getFontWidth() + getCursorSpaceBehind(),  (getFontHeight() * (1+line)), buf);     // the value can be changed with left or right

}