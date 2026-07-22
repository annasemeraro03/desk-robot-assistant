#include <Arduino.h>

#include "../src/drivers/Motor/MotorDriver.h"

/**
 * @brief Test DRV8833 motor driver.
 */
void testMotor(){
    Serial.println("[TEST] Motors");


    if(!motorDriverInit()){
        Serial.println("[FAIL] Motor initialization");
        return;
    }

    Serial.println("[OK] Motors initialized");

    Serial.println("Forward");
    setLeftMotor(MOTOR_DIRECTION_FORWARD, 150);
    setRightMotor(MOTOR_DIRECTION_FORWARD, 150);

    delay(2000);

    Serial.println("Stop");
    stopMotors();

    delay(1000);

    Serial.println("Backward");
    setLeftMotor(MOTOR_DIRECTION_BACKWARD, 150);
    setRightMotor(MOTOR_DIRECTION_BACKWARD, 150);


    delay(2000);

    stopMotors();

    Serial.println("[OK] Motor test completed");
}