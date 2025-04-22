#include "menu.h"

void mFunc_packStart(uint8_t line) {
    if (LCDML.FUNC_setup()) {
      EventArgs args;
      args.eventName = EVENT_CUSTOM_PACK_STARTUP_1;
      eventEngine.notify(args);

      // end function and go an layer back
      LCDML.FUNC_goBackToMenu(1); // leave this function and go a layer back
    }
}