#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#include <stdbool.h>

/**
 * @brief Initialize the touch sensor.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool touchInit();

/**
 * @brief Check the current touch sensor state.
 *
 * @return true if the sensor is pressed.
 * @return false otherwise.
 */
bool touchIsPressed();

#endif