#include "HardwareDiagnostics.h"

#include <Arduino.h>

#include "../I2CScanner/I2CScanner.h"
#include "../Tests/HardwareTests.h"

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

    Serial.println("\n--- I2C Scan ---");
    scanI2CBus();

    Serial.println("\n--- OLED Test ---");
    testOLED();

    Serial.println("\n--- MPU6050 Test ---");
    testMPU6050();

    Serial.println("\n--- Touch Test ---");
    testTouch();

    // Serial.println("\n--- Battery Test ---");
    // testBattery();

    // Run the motor test separately after checking the wiring.

    Serial.println("\nHardware diagnostics completed.");
}