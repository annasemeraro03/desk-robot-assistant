#ifndef I2C_SCANNER_H
#define I2C_SCANNER_H

#include <Arduino.h>
#include <Wire.h>

/**
 * @brief Scan the I2C bus and print all detected devices.
 */
void scanI2CBus();

#endif