
#include "menu.h"
#include "../animations/L33T_Animation.h"
#include "../animations/gblogo.h"

 // 1 = down, -1 is up
int XDIR = 1;
int YDIR = 1;
int gblogo_x = 0;
int gblogo_y = 0;
unsigned long previousScreenSaverMillis = 0;
// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
    int MIN_X = -20;
    int MIN_Y = -10;
    int MAX_X = 76;
    int MAX_Y = 50;
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    u8g2.firstPage();
    LCDML.FUNC_setLoopInterval(100); // starts a trigger event for the loop function every 100 milliseconds
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    int updateInterval = audioEngine.isAudioPlaying() ? 2500 : 50;
    int logoIncrease = audioEngine.isAudioPlaying() ? 5 : 1;
    if (millis() - previousScreenSaverMillis > updateInterval)
    {
      previousScreenSaverMillis = millis();
      if (!audioEngine.isAudioPlaying()) {

      gblogo_x = gblogo_x + (logoIncrease * XDIR);
      gblogo_y = gblogo_y + (logoIncrease * YDIR);
      if (gblogo_x < MIN_X) {
          XDIR = 1;
          gblogo_x = MIN_X;
      }
      if (gblogo_x > (128-MAX_X)) {
          XDIR = -1;
          gblogo_x = (128-MAX_X);
      }
      if (gblogo_y < MIN_Y) {
          YDIR = 1;
          gblogo_y = MIN_Y;
      }
      if (gblogo_y > (64-MAX_Y)) {
          YDIR = -1;
          gblogo_y = (64-MAX_Y);
      }
      } else {
        // center logo
        gblogo_x = 24;
        gblogo_y = 0;
      }
      u8g2.clearBuffer();
      u8g2.drawXBM(gblogo_x, gblogo_y, GBLOGO.getWidth(), GBLOGO.getHeight(), GBLOGODATA);
      u8g2.sendBuffer();

    }
    if (LCDML.BT_checkAny() || LCDML.REG_checkAny(_LCDML_REG_CTRL__events)) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu(); // leave this function
    }
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    // The screensaver go to the root menu
    LCDML.MENU_goRoot();
  }
}
