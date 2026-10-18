
#include <Arduino.h>
#include <Wire.h>

#include "../config/Pins.h"
#include "../config/Config.h"
#include "./I2CScanner/I2CScanner.h"
#include "./HardwareDiagnostics/HardwareDiagnostics.h"

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    delay(5000);

    Serial.println();
    Serial.println("[DEBUG 1] Setup started");

    Serial.println("[DEBUG 2] Initializing I2C");
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Serial.println("[DEBUG 3] I2C initialized");

    Serial.println("[DEBUG 4] Starting diagnostics");
    runHardwareDiagnostics();
    Serial.println("[DEBUG 5] Diagnostics completed");
}

void loop() {
    delay(1000);
}
