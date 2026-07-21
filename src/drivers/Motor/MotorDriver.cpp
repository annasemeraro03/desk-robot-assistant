#include "MotorDriver.h"

#include <Arduino.h>

#include "../../config/Pins.h"
#include "../../config/Config.h"

bool motorDriverInit(){
    // Configure motor pins
    pinMode(LEFT_MOTOR_IN1, OUTPUT);
    pinMode(LEFT_MOTOR_IN2, OUTPUT);

    pinMode(RIGHT_MOTOR_IN1, OUTPUT);
    pinMode(RIGHT_MOTOR_IN2, OUTPUT);

    // TODO:
    // Configure ESP32 LEDC PWM channels

    stopMotors();

    return true;
}

void setLeftMotor(MotorDirection direction, uint8_t speed){
    switch(direction)    {
        case MOTOR_DIRECTION_FORWARD:
            // TODO:
            // Apply PWM to IN1
            // Set IN2 LOW
            break;

        case MOTOR_DIRECTION_BACKWARD:
            // TODO:
            // Apply PWM to IN2
            // Set IN1 LOW
            break;

        case MOTOR_DIRECTION_STOP:{
            digitalWrite(LEFT_MOTOR_IN1, LOW);
            digitalWrite(LEFT_MOTOR_IN2, LOW);
            break;
        }
    }
}

void setRightMotor(MotorDirection direction, uint8_t speed){
    switch(direction){
        case MOTOR_DIRECTION_FORWARD:
            // TODO
            break;

        case MOTOR_DIRECTION_BACKWARD:
            // TODO
            break;

        case MOTOR_DIRECTION_STOP:{
            digitalWrite(RIGHT_MOTOR_IN1, LOW);
            digitalWrite(RIGHT_MOTOR_IN2, LOW);
            break;
        }
    }
}

void stopMotors(){
    setLeftMotor(
        MOTOR_DIRECTION_STOP,
        0
    );

    setRightMotor(
        MOTOR_DIRECTION_STOP,
        0
    );
}