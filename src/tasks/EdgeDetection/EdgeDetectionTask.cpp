#include "EdgeDetectionTask.h"

#include <Arduino.h>
#include <math.h>

#include "../../config/Config.h"
#include "../../app/Robot.h"
#include "../../app/RobotEvents.h"
#include "../../drivers/MPU6050/MPU6050Driver.h"

/**
 * @brief Task Periodico Real-Time tau_1: Task_IMU_Safety (Specifica Sez. 5.1)
 *        - Periodo:    T_1 = 20 ms (Deadline D_1 = 20 ms)
 *        - WCET:       C_1 = 2.0 ms
 *        - Bloccaggio: B_1 = 2.5 ms (causato al piu' da 1 pagina OLED su i2cMutex)
 *        - Risposta:   R_1 = 4.5 ms <= D_1 = 20 ms
 *        - Priorita':  Massima (6) su Core 1
 *
 * Campiona l'MPU6050 sul bus I2C condiviso proteggendo la lettura con i2cMutex.
 * Calcola le norme di accelerazione e velocita' angolare per rilevare
 * sollevamenti o sballottamenti e inviare EV_SHOCK_LIFT (MOTION_EVENT_SHAKE),
 * provocando la preemption immediata della FSM verso lo stato ARRABBIATA.
 */
void edgeDetectionTask(void *pvParameters) {
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_IMU_PERIOD_MS);

    MotionData motionData;
    uint32_t lastShockEventMs = 0;

    for (;;) {
        bool readOk = false;

        // Sezione critica su bus I2C condiviso protetta con Priority Inheritance (PIP)
        if (i2cMutex != nullptr && xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
            readOk = MPU6050Read(&motionData);
            xSemaphoreGive(i2cMutex);
        }

        if (readOk) {
            // Calcolo norma euclidea dell'accelerazione ||a|| (in quiete ~9.81 m/s^2)
            const float accelNorm = sqrtf(
                motionData.accelerationX * motionData.accelerationX +
                motionData.accelerationY * motionData.accelerationY +
                motionData.accelerationZ * motionData.accelerationZ
            );

            // Calcolo norma euclidea della velocita' angolare ||omega|| (rad/s)
            const float gyroNorm = sqrtf(
                motionData.gyroX * motionData.gyroX +
                motionData.gyroY * motionData.gyroY +
                motionData.gyroZ * motionData.gyroZ
            );

            const bool shockOrLiftDetected =
                (accelNorm < IMU_ACCEL_NORM_MIN_MS2) ||
                (accelNorm > IMU_ACCEL_NORM_MAX_MS2) ||
                (gyroNorm  > IMU_GYRO_NORM_MAX_RADS);

            const uint32_t nowMs = millis();
            if (shockOrLiftDetected && (nowMs - lastShockEventMs >= IMU_SHOCK_COOLDOWN_MS)) {
                RobotEvent ev;
                ev.type = EVENT_MOTION;
                ev.id   = MOTION_EVENT_SHAKE; // Corrisponde a EV_SHOCK_LIFT della specifica

                xQueueSend(robotEventQueue, &ev, 0);
                lastShockEventMs = nowMs;
            }
        }

        // Risveglio deterministico esattamente ogni T_1 = 20 ms
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}