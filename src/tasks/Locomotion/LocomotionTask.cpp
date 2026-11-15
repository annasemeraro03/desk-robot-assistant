#include "LocomotionTask.h"

#include <Arduino.h>

#include "../../config/Config.h"
#include "../../app/Robot.h"
#include "../../app/RobotState.h"
#include "../../drivers/Motor/MotorDriver.h"

/**
 * @brief Task Periodico Real-Time tau_3: Task_Motor_Ctrl (Specifica Sez. 5.1)
 *        - Periodo:    T_3 = 50 ms (Deadline D_3 = 50 ms)
 *        - WCET:       C_3 = 3.0 ms
 *        - Bloccaggio: B_3 = 2.5 ms
 *        - Risposta:   R_3 = 8.5 ms <= D_3 = 50 ms
 *        - Priorita':  Media-Alta (4) su Core 1
 *
 * In questa fase di verifica della FSM il task viene regolarmente schedulato
 * ogni 50 ms e acquisisce stateMutex (rispettando il modello analitico RTA),
 * mantenendo tuttavia i 4 motori N20 fermi in modalita' passiva.
 */
void locomotionTask(void *pvParameters) {
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_MOTOR_PERIOD_MS);

    RobotStateData snapshot;
    //motorStop();

    for (;;) {
        // Lettura thread-safe dello stato condiviso (C_cs,State <= 0.2 ms)
        robotStateGetSnapshot(&snapshot);

        // Modalita' passiva richiesta per questa fase: mantiene i motori fermi
       // motorStop();

        // Risveglio deterministico esattamente ogni T_3 = 50 ms
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}