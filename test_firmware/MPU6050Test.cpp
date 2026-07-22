#include <Arduino.h>

#include "../src/drivers/MPU6050/MPU6050Driver.h"

/**
 * @brief Test MPU6050 communication and data reading.
 */
void testMPU6050(){
    Serial.println("[TEST] MPU6050");

    if(!MPU6050_init()){
        Serial.println("[FAIL] MPU6050 initialization");
        return;
    }

    Serial.println("[OK] MPU6050 initialized");

    MotionData data;

    while(true){
        if(MPU6050_read(&data)){
            Serial.println("----------------");

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
        }

        delay(500);
    }
}