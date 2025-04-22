#include "common.h"

Preferences preferences;

bool prefsInitilized = false;

Preferences getPrefs() {
    if (!prefsInitilized) {
        if (preferences.begin("proton-pack", false)) {
            debugln("Prefs initialized");
            prefsInitilized = true;
        } else {
            debugln("Prefs NOT initialized.");
        }
    } else {
        debugln("Pref previously initialized.");
    }
    return preferences;
}

void closePrefs() {
    preferences.end();
    prefsInitilized = false;
}

// get_token just pulls out n value from a string of comma separated values
// for example, get_token("one,two,three", 1) would return "two"
String get_token(String &from, uint8_t index)
{
  uint16_t start = 0, idx = 0;
  uint8_t cur = 0;
  while (idx < from.length()) {
    if (from.charAt(idx) == ',') {
      if (cur == index) {
        return from.substring(start, idx);
      }
      cur++;
      while ((idx < from.length() - 1) && (from.charAt(idx + 1) == ',')) idx++;
      start = idx + 1;
    }
    idx++;
  }
  if ((cur == index) && (start < from.length())) {
    return from.substring(start, from.length());
  }
  return "";
}