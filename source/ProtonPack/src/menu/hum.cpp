#include "menu.h"
#include "../common.h"

bool isBackgroundOn = false;
void mHumming_para(uint8_t line)
// *********************************************************************
{
  isBackgroundOn = audioEngine.isBackgroundAudioOn();
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos())
  {
    // make only an action when the cursor stands on this menu item
    //check Button
    if(LCDML.BT_checkAny())
    {
      if(LCDML.BT_checkEnter())
      {
        audioEngine.toggleBackground();
        isBackgroundOn = audioEngine.isBackgroundAudioOn();
        Preferences prefs = getPrefs();
        prefs.putBool("backgroundAudio", isBackgroundOn);
        closePrefs();
        #if DEBUG == 1
          Serial.printf("saved preference %d\n", isBackgroundOn);
        #endif
        LCDML.BT_resetEnter();
      }
    }
  }

  char buf[20];
  sprintf (buf, "Backgroud: %s", isBackgroundOn ? "ON" : "OFF");

  u8g2.drawStr( getDispBoxX0() + getFontWidth() + getCursorSpaceBehind(),  (getFontHeight() * (1+line)), buf);

}