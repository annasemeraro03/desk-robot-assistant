#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include <stdint.h>

/**
 * @brief User configurable robot parameters.
 */
typedef struct{
    uint8_t focusDuration;     // Focus timer duration in minutes
    bool powerSavingEnabled;    // Enable energy saving mode
    const char* city;           // Weather location
} RobotConfig;

/**
 * @brief Global robot configuration.
 */
extern RobotConfig robotConfig;

#endif