#ifndef ROBOT_H
#define ROBOT_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/**
 * @brief Initialize the complete robot system.
 *
 * This function initializes hardware drivers,
 * creates FreeRTOS synchronization objects and
 * starts all robot tasks.
 */
void robotInit();

// Shared objects
extern QueueHandle_t robotEventQueue;
extern SemaphoreHandle_t i2cMutex;
extern SemaphoreHandle_t locomotionMutex;

#endif