#include "TouchDriver.h"

#include <Arduino.h>

#include "../../config/Pins.h"

/**
 * @brief Initialize the TTP223 touch sensor.
 *
 * Configures the sensor pin as digital input.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool touchInit(){
    pinMode(TOUCH_PIN, INPUT);      // Configure touch sensor pin
    return true;
}

/**
 * @brief Read the current touch sensor state.
 *
 * @return true if the sensor is touched.
 * @return false otherwise.
 */
bool touchIsPressed(){
    return digitalRead(TOUCH_PIN) == HIGH;
}