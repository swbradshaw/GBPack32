#include "christmas.h"
#include "../common.h"

void ChristmasRing::increment() {
    LedRing::increment();
    if (activePattern == IDLE) {
        if (color == Wheel(255)) {
            color = Wheel(85);
        } else {
            color = Wheel(255);
        }
    }
}
