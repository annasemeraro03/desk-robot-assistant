#include <Arduino.h>

#include "../../drivers/Motor/MotorDriver.h"
#include "HardwareTests.h"

static const char* MOTOR_LABELS[4] = {
    "Motor 1 (D0/D1)",
    "Motor 2 (D2/D3)",
    "Motor 3 (D10/D9)",
    "Motor 4 (D8/D7)"
};

/**
 * @brief Test DRV8833 motor driver.
 */
void testMotor(){
    Serial.println("\n==========================================");
    Serial.println("[TEST] Motors");
    Serial.println("==========================================");

    if(!motorDriverInit()){
        Serial.println("[FAIL] Motor initialization");
        return;
    }

    Serial.println("[OK] Motors initialized");

    // Test each motor individually.
    for (uint8_t i = 0; i < 4; i++) {
        Serial.print("-> Testing: ");
        Serial.println(MOTOR_LABELS[i]);

        Serial.println("   Forward");
        setMotor(i, MOTOR_DIRECTION_FORWARD, 255);
        delay(1000);

        stopMotors();
        delay(400);

        Serial.println("   Backward");
        setMotor(i, MOTOR_DIRECTION_BACKWARD, 255);
        delay(1000);

        stopMotors();
        delay(500);
    }

    // Test all motors simultaneously.
    Serial.println("-> All motors: Forward");

    for (uint8_t i = 0; i < 4; i++) {
        setMotor(i, MOTOR_DIRECTION_FORWARD, 255);
    }

    delay(300);
    stopMotors();

    delay(200);

    Serial.println("-> All motors: Backward");

    for (uint8_t i = 0; i < 4; i++) {
        setMotor(i, MOTOR_DIRECTION_BACKWARD, 255);
    }

    delay(300);
    stopMotors();

    Serial.println("[OK] Motor test completed");
}