#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

// Motor direction
typedef enum{
    MOTOR_DIRECTION_FORWARD,
    MOTOR_DIRECTION_BACKWARD,
    MOTOR_DIRECTION_STOP
} MotorDirection;

// Initialize the motor driver
bool motorDriverInit();

// Set left motor speed and direction
void setLeftMotor(
    MotorDirection direction,
    uint8_t speed
);

// Set right motor speed and direction
void setRightMotor(
    MotorDirection direction,
    uint8_t speed
);

// Stop both motors
void stopMotors();

#endif