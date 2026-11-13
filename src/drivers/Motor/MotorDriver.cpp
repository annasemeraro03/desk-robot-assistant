#include "MotorDriver.h"

#include <Arduino.h>

#include "../../config/Pins.h"
#include "../../config/Config.h"

/**
 * @brief Motor GPIO configuration.
 */
struct MotorChannel {
    uint8_t pinA;
    uint8_t pinB;
};

/**
 * @brief Pin mapping for the four motors.
 */
static const MotorChannel MOTORS[4] = {
    {PIN_MOTOR_1A, PIN_MOTOR_1B},
    {PIN_MOTOR_2A, PIN_MOTOR_2B},
    {PIN_MOTOR_3A, PIN_MOTOR_3B},
    {PIN_MOTOR_4A, PIN_MOTOR_4B}
};

// Number of motors in the system
static constexpr uint8_t MOTOR_COUNT = 4;

/**
 * @brief Initialize the motor driver hardware.
 *
 * Configures the GPIO pins connected to the DRV8833
 * motor driver and sets the motors in a safe stopped state.
 *
 * PWM configuration using ESP32 LEDC will be added here.
 *
 * @return true if initialization succeeds.
 */
bool motorDriverInit(){
     for (uint8_t i = 0; i < MOTOR_COUNT; i++) {
        pinMode(MOTORS[i].pinA, OUTPUT);
        pinMode(MOTORS[i].pinB, OUTPUT);
    }

    /*
    * Configure ESP32 LEDC PWM channels.
    *
    * PWM will be used to control motor speed by
    * changing the duty cycle applied to the motor inputs.
    */

    stopMotors();

    return true;
}

/**
 * @brief Control an individual motor.
 *
 * @param motorIndex Motor index (0-3).
 * @param direction Desired motor direction.
 * @param speed Motor speed value (0-255).
 */
void setMotor(uint8_t motorIndex, MotorDirection direction, uint8_t speed) {
    if (motorIndex >= MOTOR_COUNT) {
        return;
    }

    // TODO: implement PWM speed control
    (void)speed;

    switch (direction) {
        case MOTOR_DIRECTION_FORWARD:
            digitalWrite(MOTORS[motorIndex].pinA, HIGH);
            digitalWrite(MOTORS[motorIndex].pinB, LOW);
            break;

        case MOTOR_DIRECTION_BACKWARD:
            digitalWrite(MOTORS[motorIndex].pinA, LOW);
            digitalWrite(MOTORS[motorIndex].pinB, HIGH);
            break;

        case MOTOR_DIRECTION_STOP:
            digitalWrite(MOTORS[motorIndex].pinA, LOW);
            digitalWrite(MOTORS[motorIndex].pinB, LOW);
            break;
    }
}

/**
 * @brief Control the left motor.
 *
 * Sets the movement direction and speed of the left motor.
 * The DRV8833 controls the motor using two input signals:
 *
 * - IN1 HIGH/PWM and IN2 LOW  -> Forward
 * - IN1 LOW and IN2 HIGH/PWM  -> Backward
 * - IN1 LOW and IN2 LOW       -> Stop
 *
 * @param direction Desired motor direction.
 * @param speed Motor speed value (0-255).
 */
void setLeftMotor(MotorDirection direction, uint8_t speed){
    setMotor(0, direction, speed);
    setMotor(2, direction, speed);
}

/**
 * @brief Control the right motor.
 *
 * Sets the movement direction and speed of the right motor
 * using the DRV8833 input pins.
 *
 * @param direction Desired motor direction.
 * @param speed Motor speed value (0-255).
 */
void setRightMotor(MotorDirection direction, uint8_t speed){
    setMotor(1, direction, speed);
    setMotor(3, direction, speed);
}

/**
 * @brief Stop all motors.
 *
 * Disables the motor outputs by setting
 * both motor channels to LOW.
 */
void stopMotors(){
    for (uint8_t i = 0; i < MOTOR_COUNT; i++) {
        setMotor(i, MOTOR_DIRECTION_STOP, 0);
    }
}