#include "GraphicsTask.h"

#include <Arduino.h>

#include "../../config/Config.h"
#include "../../app/Robot.h"
#include "../../app/RobotState.h"
#include "../../drivers/OLED/OLEDDriver.h"

/**
 * @brief Task Periodico Real-Time tau_5: Task_OLED_UI (Specifica Sez. 4.2 e 5.1)
 *        - Periodo:    T_5 = 100 ms (Deadline D_5 = 100 ms)
 *        - WCET:       C_5 = 18.0 ms (8 pagine * ~2.25 ms)
 *        - Bloccaggio: B_5 = 0.2 ms (causato da stateMutex)
 *        - Risposta:   R_5 = 29.2 ms <= D_5 = 100 ms
 *        - Priorita':  Bassa (2) su Core 1
 *
 * Implementa la Soluzione Software Real-Time (Page-Chunking + Priority Inheritance):
 *   1. Acquisisce stateMutex per < 0.2 ms per copiare uno snapshot dello stato.
 *   2. Disegna la schermata nel frame buffer in RAM da 1024 byte (zero traffico I2C).
 *   3. Cicla sulle 8 pagine hardware da 128 byte ciascuna: per ogni pagina acquisisce
 *      i2cMutex, trasmette i 128 byte (~2.3 ms), rilascia i2cMutex e invoca taskYIELD().
 *      In questo modo il task ad alta priorita' tau_1 (Task_IMU_Safety, D_1 = 20 ms)
 *      subisce un bloccaggio massimo B_1 <= 2.5 ms anziche' 23 ms.
 */
void graphicsTask(void *pvParameters) {
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_OLED_PERIOD_MS);

    RobotStateData snapshot;

    for (;;) {
        // 1. Legge lo stato corrente acquisendo stateMutex (C_cs,State <= 0.2 ms)
        robotStateGetSnapshot(&snapshot);

        // 2. Compone la schermata di stato nel frame buffer locale in RAM
        oledRenderStateScreen(&snapshot);

        // 3. Trasferimento I2C frammentato in 8 pagine da 128 byte (Page-Chunking)
        for (uint8_t page = 0; page < DISPLAY_PAGES; ++page) {
            if (i2cMutex != nullptr && xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
                oledUpdatePage(page); // Sezione critica limitata a ~2.3 ms <= 2.5 ms
                xSemaphoreGive(i2cMutex);
            }
            // Cede volontariamente la CPU tra una pagina e l'altra per permettere
            // l'esecuzione immediata di eventuali task a priorita' superiore pronti
            taskYIELD();
        }

        // 4. Risveglio deterministico esattamente ogni T_5 = 100 ms
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}