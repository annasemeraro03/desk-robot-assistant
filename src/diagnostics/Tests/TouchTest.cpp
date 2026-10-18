
#include <Arduino.h>
#include "../../drivers/Touch/TouchDriver.h"

/**
 * @brief Test TTP223 touch sensor once.
 */
void testTouch() {
    Serial.println("[TEST] Touch");

    if (!touchInit()) {
        Serial.println("[FAIL] Touch initialization");
        return;
    }

    Serial.println("[OK] Touch initialized");

    if (touchIsPressed()) {
        Serial.println("[OK] Touch detected");
    } else {
        Serial.println("[OK] Touch sensor responding, not pressed");
    }

    Serial.println("[TEST] Touch completed");
}
