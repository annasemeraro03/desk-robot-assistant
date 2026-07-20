#ifndef MPU6050_DRIVER_H
#define MPU6050_DRIVER_H

struct MotionData {
    float accelerationX;
    float accelerationY;
    float accelerationZ;

    float gyroX;
    float gyroY;
    float gyroZ;
};

// Initialize MPU6050 sensor
bool MPU6050_init();

// Read accelerometer and gyroscope data
bool MPU6050_read(MotionData *data);

#endif