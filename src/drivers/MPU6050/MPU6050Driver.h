#ifndef MPU6050_DRIVER_H
#define MPU6050_DRIVER_H

/**
 * @brief Structure containing MPU6050 sensor data.
 */
struct MotionData {
    float accelerationX;
    float accelerationY;
    float accelerationZ;

    float gyroX;
    float gyroY;
    float gyroZ;
};

/**
 * @brief Initialize the MPU6050 sensor.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool MPU6050Init();

/**
 * @brief Read accelerometer and gyroscope data.
 *
 * @param data Pointer to the structure where sensor data is stored.
 *
 * @return true if data reading succeeds.
 * @return false otherwise.
 */
bool MPU6050Read(MotionData *data);

#endif