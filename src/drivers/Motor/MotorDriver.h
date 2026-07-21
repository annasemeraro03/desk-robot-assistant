#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

/**
 * @brief Motor movement direction.
 */
typedef enum{
    MOTOR_DIRECTION_FORWARD,
    MOTOR_DIRECTION_BACKWARD,
    MOTOR_DIRECTION_STOP
} MotorDirection;

/**
 * @brief Initialize the motor driver.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool motorDriverInit();

/**
 * @brief Set left motor speed and direction.
 *
 * @param direction Motor movement direction.
 * @param speed Motor speed value (0-255).
 */
void setLeftMotor(MotorDirection direction, uint8_t speed);

/**
 * @brief Set right motor speed and direction.
 *
 * @param direction Motor movement direction.
 * @param speed Motor speed value (0-255).
 */
void setRightMotor(MotorDirection direction, uint8_t speed);

/**
 * @brief Stop both motors.
 */
void stopMotors();

#endif