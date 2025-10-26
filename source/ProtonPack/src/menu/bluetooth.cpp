#include "menu.h"
#include "../bluetooth/bluetooth.h"
#include "../common.h"

void mBluetooth_para(uint8_t line)
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
        if (isBluetoothOn()) {
          Serial.println("stop bluetooth - rebooting");
          ESP.restart();
        } else {
          Serial.println("enter, start bluetooth");
          audioEngine.shutdownAudioEngine();
          delay(100);
          startBluetooth();
        }

        LCDML.BT_resetEnter();
      }
    }
  }

  char buf[20];
  sprintf (buf, "Bluetooth: %s", isBluetoothOn() ? "ON" : "OFF");

  // setup function
  u8g2.drawStr( getDispBoxX0() + getFontWidth() + getCursorSpaceBehind(),  (getFontHeight() * (1+line)), buf);     // the value can be changed with left or right

}