#ifndef BATTERY_DRIVER_H
#define BATTERY_DRIVER_H

#include <stdint.h>

/**
 * @brief Initialize the battery monitoring system.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool batteryInit();                

/**
 * @brief Read the current battery voltage.
 *
 * @return Battery voltage in volts.
 */
float batteryGetVoltage();    

/**
 * @brief Get the estimated battery charge level.
 *
 * @return Battery level percentage (0-100).
 */
uint8_t batteryGetPercentage();    

/**
 * @brief Check if the battery voltage is below the threshold.
 *
 * @return true if the battery needs charging.
 * @return false otherwise.
 */
bool batteryIsLow();              

#endif