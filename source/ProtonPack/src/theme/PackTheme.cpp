#include "PackTheme.h"
#include "common.h"

PackTheme::PackTheme() {
}

void PackTheme::setTheme(int themeNumber) {
    currentTheme = themeNumber;
}

void PackTheme::loadTheme() {
    // load theme from JSON

    // wand colors, folder info, sounds, behavior, preferences, etc
}

void PackTheme::notify(EventArgs args) {
    handleEvent(args);
}

void PackTheme::handleEvent(EventArgs args) {
    // listen for theme changes
}
