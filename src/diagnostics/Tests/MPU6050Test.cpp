
#include <Arduino.h>
#include "../../drivers/MPU6050/MPU6050Driver.h"

/**
 * @brief Test MPU6050 initialization and data reading once.
 */
void testMPU6050() {
    Serial.println("[TEST] MPU6050");

    if (!MPU6050Init()) {
        Serial.println("[FAIL] MPU6050 initialization");
        return;
    }

    Serial.println("[OK] MPU6050 initialized");

    MotionData data;

    if (MPU6050Read(&data)) {
        Serial.println("[OK] MPU6050 data read successfully");

        Serial.print("Accel X: ");
        Serial.println(data.accelerationX);

        Serial.print("Accel Y: ");
        Serial.println(data.accelerationY);

        Serial.print("Accel Z: ");
        Serial.println(data.accelerationZ);

        Serial.print("Gyro X: ");
        Serial.println(data.gyroX);

        Serial.print("Gyro Y: ");
        Serial.println(data.gyroY);

        Serial.print("Gyro Z: ");
        Serial.println(data.gyroZ);
    } else {
        Serial.println("[FAIL] Could not read MPU6050 data");
    }

    Serial.println("[TEST] MPU6050 completed");
}
