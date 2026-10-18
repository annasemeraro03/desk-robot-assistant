
#include "MPU6050Driver.h"

#include <Arduino.h>
#include <Wire.h>

#include "../../config/Pins.h"

static constexpr uint8_t MPU_ADDRESS = 0x68;

static bool writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
}

static bool readRegisters(uint8_t reg, uint8_t *buffer, size_t length) {
    if (buffer == nullptr || length == 0 || length > 32) {
        return false;
    }

    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(reg);

    if (Wire.endTransmission(false) != 0) {
        return false;
    }

    if (Wire.requestFrom(MPU_ADDRESS, (uint8_t)length) != length) {
        while (Wire.available()) {
            Wire.read();
        }
        return false;
    }

    for (size_t i = 0; i < length; ++i) {
        buffer[i] = Wire.read();
    }

    return true;
}

static int16_t combineBytes(uint8_t high, uint8_t low) {
    return (int16_t)(((uint16_t)high << 8) | low);
}

bool MPU6050Init() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    Serial.println("[MPU] Initializing through raw I2C");

    Wire.beginTransmission(MPU_ADDRESS);
    if (Wire.endTransmission() != 0) {
        Serial.println("[MPU] Device not responding at 0x68");
        return false;
    }

    uint8_t identity = 0;
    if (!readRegisters(0x75, &identity, 1)) {
        Serial.println("[MPU] Cannot read WHO_AM_I");
        return false;
    }

    Serial.print("[MPU] WHO_AM_I = 0x");
    Serial.println(identity, HEX);

    if (identity != 0x68 && identity != 0x70) {
        Serial.println("[MPU] Unknown identity; refusing initialization");
        return false;
    }

    // Select the X-axis gyroscope PLL clock and wake the device.
    if (!writeRegister(0x6B, 0x01)) {
        Serial.println("[MPU] Failed to wake device");
        return false;
    }

    delay(100);

    // Disable the auxiliary I2C master.
    if (!writeRegister(0x6A, 0x00)) {
        Serial.println("[MPU] Failed to configure USER_CTRL");
        return false;
    }

    // Configure digital low-pass filter.
    if (!writeRegister(0x1A, 0x04)) {
        Serial.println("[MPU] Failed to configure filter");
        return false;
    }

    // Accelerometer range: +/-8 g.
    if (!writeRegister(0x1C, 0x10)) {
        Serial.println("[MPU] Failed to configure accelerometer");
        return false;
    }

    // Gyroscope range: +/-500 degrees per second.
    if (!writeRegister(0x1B, 0x08)) {
        Serial.println("[MPU] Failed to configure gyroscope");
        return false;
    }

    delay(50);

    uint8_t check = 0;

    if (!readRegisters(0x1C, &check, 1) || check != 0x10) {
        Serial.println("[MPU] Accelerometer configuration verification failed");
        return false;
    }

    if (!readRegisters(0x1B, &check, 1) || check != 0x08) {
        Serial.println("[MPU] Gyroscope configuration verification failed");
        return false;
    }

    Serial.println("[MPU] Raw I2C initialization successful");
    return true;
}

bool MPU6050Read(MotionData *data) {
    if (data == nullptr) {
        return false;
    }

    uint8_t accel[6];
    uint8_t gyro[6];

    if (!readRegisters(0x3B, accel, sizeof(accel))) {
        Serial.println("[MPU] Accelerometer read failed");
        return false;
    }

    if (!readRegisters(0x43, gyro, sizeof(gyro))) {
        Serial.println("[MPU] Gyroscope read failed");
        return false;
    }

    // Convert acceleration from raw counts to m/s^2.
    constexpr float ACCEL_SCALE = 9.80665f / 4096.0f;

    data->accelerationX = combineBytes(accel[0], accel[1]) * ACCEL_SCALE;
    data->accelerationY = combineBytes(accel[2], accel[3]) * ACCEL_SCALE;
    data->accelerationZ = combineBytes(accel[4], accel[5]) * ACCEL_SCALE;

    // Convert gyroscope readings from raw counts to radians/second.
    constexpr float GYRO_SCALE = 0.01745329252f / 65.5f;

    data->gyroX = combineBytes(gyro[0], gyro[1]) * GYRO_SCALE;
    data->gyroY = combineBytes(gyro[2], gyro[3]) * GYRO_SCALE;
    data->gyroZ = combineBytes(gyro[4], gyro[5]) * GYRO_SCALE;

    return true;
}
