#include "NetworkTask.h"

#include <Arduino.h>

#include "../../config/Config.h"
#include "../../app/Robot.h"
#include "../../app/RobotEvents.h"
#include "../../app/RobotConfig.h"

/**
 * @brief Task Periodico Real-Time tau_6: Task_Sync_Cal (Specifica Sez. 5.1)
 *        - Periodo:    T_6 = 500 ms (Deadline D_6 = 500 ms)
 *        - WCET:       C_6 = 15.0 ms
 *        - Bloccaggio: B_6 = 0.0 ms (nessun task a priorita' inferiore)
 *        - Risposta:   R_6 = 46.0 ms <= D_6 = 500 ms
 *        - Priorita':  Minima (1) su Core 1
 *
 * Controlla le scadenze del calendario e legge in modo non bloccante il Monitor Seriale:
 *   - Digitando 'c' o 'C' sulla seriale del PC inietta EV_CALENDAR_ALERT nella coda
 *     della FSM per testare subito la transizione e la preemption di INFORMAZIONI_CALENDARIO.
 *   - Sono inoltre disponibili i tasti 's' (EV_SHOCK_LIFT), '1' (EV_TAP_1),
 *     '2' (EV_TAP_2), '3' (EV_TAP_3) e 'p' (EV_PETTING) per agevolare ogni test da PC.
 */
void networkTask(void *pvParameters) {
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_SYNC_PERIOD_MS);

    for (;;) {
        // Lettura non bloccante dei comandi da Monitor Seriale per test e sincronizzazione
        while (Serial.available() > 0) {
            const char cmd = (char)Serial.read();
            RobotEvent ev;
            bool send = false;

            switch (cmd) {
                case 'c':
                case 'C':
                    // Trigger richiesto al punto 2: inietta EV_CALENDAR_ALERT (-10 min)
                    ev.type = EVENT_CALENDAR;
                    ev.id   = CALENDAR_EVENT_REMINDER;
                    send    = true;
                    Serial.println("[SERIAL TRIGGER] Iniettato EV_CALENDAR_ALERT (-10 min)");
                    break;

                case 's':
                case 'S':
                    // Simula sollevamento/sballottamento MPU6050 -> Preemption ARRABBIATA
                    ev.type = EVENT_MOTION;
                    ev.id   = MOTION_EVENT_SHAKE;
                    send    = true;
                    Serial.println("[SERIAL TRIGGER] Iniettato EV_SHOCK_LIFT (Shake)");
                    break;

                case '1':
                    ev.type = EVENT_TOUCH;
                    ev.id   = TOUCH_EVENT_SINGLE;
                    send    = true;
                    Serial.println("[SERIAL TRIGGER] Iniettato EV_TAP_1 (1 Tap)");
                    break;

                case '2':
                    ev.type = EVENT_TOUCH;
                    ev.id   = TOUCH_EVENT_DOUBLE;
                    send    = true;
                    Serial.println("[SERIAL TRIGGER] Iniettato EV_TAP_2 (2 Tap)");
                    break;

                case '3':
                    ev.type = EVENT_TOUCH;
                    ev.id   = TOUCH_EVENT_TRIPLE;
                    send    = true;
                    Serial.println("[SERIAL TRIGGER] Iniettato EV_TAP_3 (3 Tap)");
                    break;

                case 'p':
                case 'P':
                    ev.type = EVENT_TOUCH;
                    ev.id   = TOUCH_EVENT_LONG;
                    send    = true;
                    Serial.println("[SERIAL TRIGGER] Iniettato EV_PETTING (Carezza > 800ms)");
                    break;

                default:
                    break;
            }

            if (send && robotEventQueue != nullptr) {
                xQueueSend(robotEventQueue, &ev, 0);
            }
        }

        // Risveglio deterministico esattamente ogni T_6 = 500 ms
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}