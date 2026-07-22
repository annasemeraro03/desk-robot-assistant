#include <Arduino.h>

#include "../src/drivers/Touch/TouchDriver.h"

/**
 * @brief Test TTP223 touch sensor.
 */
void testTouch(){
    Serial.println("[TEST] Touch");

    if(!touchInit()){
        Serial.println("[FAIL] Touch initialization");
        return;
    }

    Serial.println("[OK] Touch initialized");

    while(true){
        if(touchIsPressed()){
            Serial.println("Touch detected");
        }

        delay(100);
    }
}