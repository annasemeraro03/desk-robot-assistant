#include <Arduino.h>

#include "../src/drivers/Battery/BatteryDriver.h"

/**
 * @brief Test battery voltage measurement.
 */
void testBattery(){
    Serial.println("[TEST] Battery");

    if(!batteryInit()){
        Serial.println("[FAIL] Battery initialization");
        return;
    }

    Serial.println("[OK] Battery initialized");

    while(true){
        float voltage = batteryGetVoltage();

        uint8_t percentage = batteryGetPercentage();

        Serial.print("Voltage: ");
        Serial.print(voltage);
        Serial.println(" V");

        Serial.print("Charge: ");
        Serial.print(percentage);
        Serial.println("%");

        if(batteryIsLow()){
            Serial.println("WARNING: LOW BATTERY");
        }

        delay(1000);
    }
}