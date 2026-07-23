#ifndef ROBOT_STATE_H
#define ROBOT_STATE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Robot emotional states.
 */
typedef enum: uint8_t{
    MOOD_HAPPY,
    MOOD_BORED,
    MOOD_FOCUS,
    MOOD_SLEEPY,
    MOOD_LOW_BATTERY,
    MOOD_SURPRISED,
    MOOD_DIZZY,
    MOOD_CHARMING
} RobotMood;

/**
 * @brief Robot operating modes.
 */
typedef enum: uint8_t{
    MODE_NORMAL,
    MODE_FOCUS,
    MODE_POWER_SAVE,
    MODE_SLEEP
} RobotMode;

/**
 * @brief OLED display pages.
 */
typedef enum: uint8_t{
    PAGE_EMOTION,
    PAGE_CLOCK,
    PAGE_WEATHER
} UIPage;

/**
 * @brief Robot movement commands.
 */
typedef enum{
    MOVEMENT_STOP,
    MOVEMENT_FORWARD,
    MOVEMENT_BACKWARD,
    MOVEMENT_LEFT,
    MOVEMENT_RIGHT,
    MOVEMENT_RANDOM
} RobotMovement;

/**
 * @brief Complete robot runtime state.
 */
typedef struct{
    RobotMood mood;
    RobotMode mode;
    RobotMovement movement;

    UIPage currentPage;

    float energy;
    float attention;
    bool focusActive;
} RobotState;

/**
 * @brief Global robot state.
 */
extern RobotState robotState;

#endif