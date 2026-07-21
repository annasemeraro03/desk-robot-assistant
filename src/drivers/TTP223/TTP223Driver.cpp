#include "TouchDriver.h"

#include <Arduino.h>

#include "../../config/Pins.h"

// Initialize the touch sensor
bool touchInit(){
    pinMode(TOUCH_PIN, INPUT);
    return true;
}

// Read the touch sensor state
bool touchIsPressed(){return digitalRead(TOUCH_PIN) == HIGH;}