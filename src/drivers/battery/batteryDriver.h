#ifndef BATTERY_DRIVER_H
#define BATTERY_DRIVER_H

#include <stdint.h>

// Initialize the battery monitoring system
bool batteryInit();

// Read the battery voltage
float batteryGetVoltage();

// Read the battery charge level (0 - 100%)
uint8_t batteryGetPercentage();

// Check if the battery level is below the threshold
bool batteryIsLow();

#endif