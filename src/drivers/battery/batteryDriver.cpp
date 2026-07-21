#include "BatteryDriver.h"

#include <Arduino.h>

#include "../../config/Pins.h"
#include "../../config/Config.h"

// STATIC VARIABLES
static constexpr float ADC_REFERENCE_VOLTAGE = 3.3f;    // ADC reference voltage
static constexpr uint16_t ADC_MAX_VALUE = 4095;         // ESP32 ADC resolution (12-bit)

// Initialize battery monitoring
bool batteryInit(){
    pinMode(BATTERY_ADC_PIN, INPUT);

    return true;
}

// Read battery voltage
float batteryGetVoltage(){
    uint16_t adcValue = analogRead(BATTERY_ADC_PIN);

    float adcVoltage =
        (static_cast<float>(adcValue) / ADC_MAX_VALUE) *
        ADC_REFERENCE_VOLTAGE;

    // TODO:
    // Apply the voltage divider ratio used in the hardware.
    // Example:
    // return adcVoltage * 2.0f;

    return adcVoltage;
}

// Estimate battery charge percentage
uint8_t batteryGetPercentage(){
    float voltage = batteryGetVoltage();

    // Clamp the voltage range
    if (voltage <= 3.0f)
    {
        return 0;
    }

    if (voltage >= 4.2f)
    {
        return 100;
    }

    return static_cast<uint8_t>(
        ((voltage - 3.0f) / (4.2f - 3.0f)) * 100.0f
    );
}

// Check if the battery is below the low-voltage threshold
bool batteryIsLow(){return batteryGetVoltage() <= LOW_BATTERY_VOLTAGE;}