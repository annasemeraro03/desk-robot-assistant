#include "MPU6050Driver.h"

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "../../config/Pins.h"

// ===============================
//      MPU6050 Instance
// ===============================

// MPU6050 sensor instance
static Adafruit_MPU6050 mpu;

/**
 * @brief Initialize the MPU6050 sensor.
 *
 * Initializes the I2C communication and configures
 * the accelerometer, gyroscope and filter parameters.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool MPU6050Init() {
    // Initialize I2C communication
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    // Initialize MPU6050
    if (!mpu.begin()){
        Serial.println("MPU6050 initialization failed");
        return false;
    }

    // Configure sensor parameters
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("MPU6050 initialized successfully");
    return true;
}

/**
 * @brief Read accelerometer and gyroscope data.
 *
 * Reads the current sensor values and stores them
 * into the provided MotionData structure.
 *
 * @param data Pointer to the structure where
 * sensor data will be stored.
 *
 * @return true if data reading succeeds.
 * @return false if the input pointer is invalid.
 */
bool MPU6050Read(MotionData *data){
     // Check if output structure is valid
    if (data == nullptr) return false;

    sensors_event_t acceleration;
    sensors_event_t rotation;
    sensors_event_t temperature;

    // Read sensor values
    mpu.getEvent(&acceleration, &rotation, &temperature);

    // Store acceleration values
    data->accelerationX = acceleration.acceleration.x;
    data->accelerationY = acceleration.acceleration.y;
    data->accelerationZ = acceleration.acceleration.z;

    // Store gyroscope values
    data->gyroX = rotation.gyro.x;
    data->gyroY = rotation.gyro.y;
    data->gyroZ = rotation.gyro.z;

    return true;
}