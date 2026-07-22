#ifndef CONFIG_H
#define CONFIG_H

// ===============================
// Serial Communication
// ===============================

#define SERIAL_BAUDRATE 115200

// ===============================
// Display Configuration
// ===============================

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_REFRESH_PERIOD_MS 33
#define DISPLAY_RESET -1
#define DISPLAY_I2C_ADDRESS 0x3C

// ===============================
// Battery Configuration
// ===============================

#define BATTERY_CHECK_PERIOD_MS 5000
#define LOW_BATTERY_VOLTAGE 3.4

// ===============================
// Network Configuration
// ===============================

#define WEATHER_UPDATE_PERIOD_MS 300000

#endif