#ifndef ROBOT_H
#define ROBOT_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/**
 * @brief Inizializza l'intero sistema Real-Time del Robot Emotivo:
 *        - Driver hardware (Touch, Motori, OLED SH1106, IMU MPU6050)
 *        - Semafori Mutex con Priority Inheritance Protocol (i2cMutex, stateMutex)
 *        - Coda eventi della FSM (robotEventQueue)
 *        - I 6 task periodici allocati in isolamento sul Core 1 (APP_CPU)
 *          con priorita' Rate Monotonic (Specifica Sez. 4.1 e 5.1).
 */
void robotInit();

// Oggetti di sincronizzazione condivisi (Specifica Sez. 4.2 e 5.2)
extern QueueHandle_t robotEventQueue;
extern SemaphoreHandle_t i2cMutex;         // mutexI2C   (C_cs,I2C <= 2.5 ms)
extern SemaphoreHandle_t stateMutex;       // mutexState (C_cs,State <= 0.2 ms)
extern SemaphoreHandle_t locomotionMutex;  // Retrocompatibilita'

#endif