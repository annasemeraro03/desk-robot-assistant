#include "HardwareDiagnostics.h"

#include <Arduino.h>

#include "../I2CScanner/I2CScanner.h"

/**
 * @brief Execute all available hardware diagnostics.
 *
 * This function runs a sequence of hardware tests during startup.
 * It is intended for development and debugging purposes.
 */
void runHardwareDiagnostics(){

    Serial.println();
    Serial.println("========================================");
    Serial.println(" Desk Robot Hardware Diagnostics");
    Serial.println("========================================");

    scanI2CBus();

    // TODO:
    // Verify OLED initialization.

    // TODO:
    // Verify MPU6050 communication.

    // TODO:
    // Verify touch sensor.

    // TODO:
    // Verify battery ADC.

    // TODO:
    // Verify motor driver.

    Serial.println();
    Serial.println("Hardware diagnostics completed.");
    Serial.println();
}