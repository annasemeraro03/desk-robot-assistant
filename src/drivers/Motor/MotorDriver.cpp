#include "MotorDriver.h"

#include <Arduino.h>

#include "../../config/Pins.h"
#include "../../config/Config.h"

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
    // Configure motor pins
    pinMode(LEFT_MOTOR_IN1, OUTPUT);
    pinMode(LEFT_MOTOR_IN2, OUTPUT);

    pinMode(RIGHT_MOTOR_IN1, OUTPUT);
    pinMode(RIGHT_MOTOR_IN2, OUTPUT);

    /*
     * Configure ESP32 LEDC PWM channels.
     *
     * PWM will be used to control motor speed by
     * changing the duty cycle applied to the motor inputs.
     */

    stopMotors();       // Ensure motors are stopped after initialization

    return true;
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
    switch(direction)    {
        case MOTOR_DIRECTION_FORWARD: {
            /*
             * TO DO:
             * Apply PWM signal to IN1.
             * Set IN2 LOW.
             */
            break;
        }

        case MOTOR_DIRECTION_BACKWARD: {
            /*
             * TO DO:
             * Apply PWM signal to IN2.
             * Set IN1 LOW.
             */
            break;
        }

        case MOTOR_DIRECTION_STOP: {
            // Disable both motor inputs
            digitalWrite(LEFT_MOTOR_IN1, LOW);
            digitalWrite(LEFT_MOTOR_IN2, LOW);
            break;
        }
    }
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
    switch(direction){
        case MOTOR_DIRECTION_FORWARD:
            /*
             * TO DO:
             * Apply PWM signal to IN1.
             * Set IN2 LOW.
             */
            break;

        case MOTOR_DIRECTION_BACKWARD:
            /*
             * TO DO:
             * Apply PWM signal to IN2.
             * Set IN1 LOW.
             */
            break;

        case MOTOR_DIRECTION_STOP:{
            // Disable both motor inputs
            digitalWrite(RIGHT_MOTOR_IN1, LOW);
            digitalWrite(RIGHT_MOTOR_IN2, LOW);
            break;
        }
    }
}

/**
 * @brief Stop all motors.
 *
 * Disables the motor outputs by setting
 * both motor channels to LOW.
 */
void stopMotors(){
    setLeftMotor(MOTOR_DIRECTION_STOP, 0);
    setRightMotor(MOTOR_DIRECTION_STOP, 0);
}