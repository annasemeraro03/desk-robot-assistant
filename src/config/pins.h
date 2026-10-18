#ifndef PINS_H
#define PINS_H


// ===============================
//      Touch Sensor (TTP223)
// ===============================

#define TOUCH_PIN D6

// ===============================
//          I2C Bus
//      OLED SH1106 + MPU6050
// ===============================

#define I2C_SDA_PIN D4
#define I2C_SCL_PIN D5

// ===============================
//      Motor Driver DRV8833
// ===============================

// Left Motors
#define LEFT_MOTOR_IN1 D0
#define LEFT_MOTOR_IN2 D1

// Right Motors
#define RIGHT_MOTOR_IN1 D7
#define RIGHT_MOTOR_IN2 D8

// ===============================
// Battery Voltage Measurement
// ===============================

#define BATTERY_ADC_PIN 0

#endif