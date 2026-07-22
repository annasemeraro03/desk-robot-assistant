#include <Arduino.h>

#include "../src/drivers/OLED/OLEDDriver.h"

/**
 * @brief Test OLED display initialization and rendering.
 */
void testOLED(){
    Serial.println("[TEST] OLED");

    if(!oledInit()){
        Serial.println("[FAIL] OLED initialization");
        return;
    }

    Serial.println("[OK] OLED initialized");

    oledClear();

    oledDrawText(0, 0, "Desk Robot");
    oledDrawText(0, 20, "OLED TEST OK");

    oledUpdate();

    Serial.println("[OK] OLED display test completed");
}