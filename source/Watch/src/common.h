#pragma once

#include "Arduino.h"
#include <Preferences.h>

Preferences getPrefs();
void closePrefs();
String get_token(String &from, uint8_t index);

/* DEBUG information */
bool const DEBUG = true;
#define DEBUG 1

// Debug macros
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

