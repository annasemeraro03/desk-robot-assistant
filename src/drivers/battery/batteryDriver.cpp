#include "BatteryDriver.h"

#include <Arduino.h>

#include "../../config/Pins.h"
#include "../../config/Config.h"

// ===============================
//      ADC Configuration
// ===============================
static constexpr float ADC_REFERENCE_VOLTAGE = 3.3f;    // ADC reference voltage
static constexpr uint16_t ADC_MAX_VALUE = 4095;         // ESP32 ADC resolution (12-bit)

/**
 * @brief Initialize the battery monitoring system.
 *
 * Configures the ADC pin used to measure the battery voltage.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool batteryInit(){
    pinMode(BATTERY_ADC_PIN, INPUT);        // Configure battery measurement pin as input
    return true;
}


/**
 * @brief Read the current battery voltage.
 *
 * The ESP32 ADC measures the voltage coming from
 * the battery voltage divider. The ADC value is
 * converted into a voltage value.
 *
 * @return Battery voltage in volts.
 */
float batteryGetVoltage(){
    uint16_t adcValue = analogRead(BATTERY_ADC_PIN);        // Read raw ADC value

    // Convert ADC value into voltage
    float adcVoltage = (static_cast<float>(adcValue) / ADC_MAX_VALUE) * ADC_REFERENCE_VOLTAGE;

    /*
     * Apply voltage divider compensation.
     *
     * The battery voltage is reduced by the
     * voltage divider before reaching the ADC pin.
     *
     * Example:
     *
     * return adcVoltage * 2.0f;
     */

    return adcVoltage;
}

/**
 * @brief Estimate the battery charge percentage.
 *
 * The estimation is based on the LiPo battery voltage range:
 * - 3.0V -> 0%
 * - 4.2V -> 100%
 *
 * @return Battery level percentage (0-100).
 */
uint8_t batteryGetPercentage(){
    float voltage = batteryGetVoltage();

    // Clamp the voltage range
    if (voltage <= 3.0f) return 0;          // Battery voltage below minimum level
    if (voltage >= 4.2f) return 100;        // Battery fully charged

    // Linear approximation of battery percentage
    return static_cast<uint8_t>(((voltage - 3.0f) / (4.2f - 3.0f)) * 100.0f);
}

/**
 * @brief Check whether the battery level is low.
 *
 * Compares the measured battery voltage with the configured low-voltage threshold.
 *
 * @return true if the battery needs charging.
 * @return false otherwise.
 */
bool batteryIsLow(){
    return batteryGetVoltage() <= LOW_BATTERY_VOLTAGE;
}