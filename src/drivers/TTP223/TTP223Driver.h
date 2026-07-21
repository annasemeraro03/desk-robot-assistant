#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#include <stdbool.h>

// Initialize the touch sensor
bool touchInit();

// Read the current touch state
bool touchIsPressed();

#endif