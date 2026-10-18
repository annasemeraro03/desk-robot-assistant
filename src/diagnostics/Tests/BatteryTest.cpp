
#include <Arduino.h>
#include "../../drivers/Battery/BatteryDriver.h"

/**
 * @brief Test battery voltage measurement once.
 */
void testBattery() {
    Serial.println("[TEST] Battery");

    if (!batteryInit()) {
        Serial.println("[FAIL] Battery initialization");
        return;
    }

    Serial.println("[OK] Battery initialized");

    float voltage = batteryGetVoltage();
    uint8_t percentage = batteryGetPercentage();

    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");

    Serial.print("Charge: ");
    Serial.print(percentage);
    Serial.println("%");

    if (batteryIsLow()) {
        Serial.println("[WARNING] LOW BATTERY");
    } else {
        Serial.println("[OK] Battery level is sufficient");
    }

    Serial.println("[TEST] Battery completed");
}
