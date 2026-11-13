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
// --- Pinout 4 Motori N20 su 2x DRV8833 (Specifica Sez. 2.2) ---
#ifndef PIN_MOTOR_1A
// DRV8833 #1 (Superiore)
#define PIN_MOTOR_1A D0  // IN1 -> Motore 1 (OUT1/OUT2)
#define PIN_MOTOR_1B D1  // IN2 -> Motore 1 (OUT1/OUT2)
#define PIN_MOTOR_2A D2  // IN3 -> Motore 2 (OUT3/OUT4)
#define PIN_MOTOR_2B D3  // IN4 -> Motore 2 (OUT3/OUT4)

// DRV8833 #2 (Inferiore)
#define PIN_MOTOR_4B D7  // IN4 -> Motore 4 (OUT3/OUT4)
#define PIN_MOTOR_4A D8  // IN3 -> Motore 4 (OUT3/OUT4)
#define PIN_MOTOR_3B D9  // IN2 -> Motore 3 (OUT1/OUT2)
#define PIN_MOTOR_3A D10 // IN1 -> Motore 3 (OUT1/OUT2)

// Alias di retrocompatibilità per il vecchio MotorDriver.cpp / HardwareDiagnostics
#define LEFT_MOTOR_IN1  PIN_MOTOR_1A
#define LEFT_MOTOR_IN2  PIN_MOTOR_1B
#define RIGHT_MOTOR_IN1 PIN_MOTOR_2A
#define RIGHT_MOTOR_IN2 PIN_MOTOR_2B
#endif

// ===============================
// Battery Voltage Measurement
// ===============================
#define BATTERY_ADC_PIN 0

#endif