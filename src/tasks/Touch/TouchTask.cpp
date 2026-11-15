#include "TouchTask.h"

#include <Arduino.h>

#include "../../config/Config.h"
#include "../../drivers/Touch/TouchDriver.h"
#include "../../app/RobotEvents.h"
#include "../../app/Robot.h"

/**
 * @brief Invia un evento touch classificato alla coda eventi della FSM.
 */
static void sendTouchEvent(TouchEvent eventId) {
    RobotEvent event;
    event.type = EVENT_TOUCH;
    event.id   = eventId;

    xQueueSend(robotEventQueue, &event, 0);
}

/**
 * @brief Task Periodico Real-Time tau_2: Task_Touch_Input (Specifica Sez. 3.2.A e 5.1)
 *        - Periodo:    T_2 = 25 ms (Deadline D_2 = 25 ms)
 *        - WCET:       C_2 = 1.0 ms
 *        - Bloccaggio: B_2 = 2.5 ms (push-through blocking da PIP)
 *        - Risposta:   R_2 = 5.5 ms <= D_2 = 25 ms
 *        - Priorita':  Alta (5) su Core 1
 *
 * Implementa la grammatica temporale sul sensore TTP223 (pin D6):
 *   - Tap breve: 50 ms <= t_on < 400 ms con finestra inter-tap t_gap = 350 ms
 *     -> emette EV_TAP_1, EV_TAP_2, EV_TAP_3
 *   - Carezza prolungata: livello HIGH continuo per t_on >= 800 ms
 *     -> emette immediatamente EV_PETTING (TOUCH_EVENT_LONG)
 */
void touchTask(void *pvParameters) {
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_TOUCH_PERIOD_MS);

    bool previousState     = false;
    bool longPressDetected = false;

    uint32_t pressStartMs  = 0;
    uint32_t lastReleaseMs = 0;
    uint8_t tapCount       = 0;

    for (;;) {
        const bool currentState = touchIsPressed();
        const uint32_t now      = millis();

        // 1. Fronte di salita (inizio pressione sul TTP223)
        if (currentState && !previousState) {
            pressStartMs      = now;
            longPressDetected = false;
        }

        // 2. Rilevamento carezza / coccola prolungata (t_on >= 800 ms)
        if (currentState && !longPressDetected) {
            if (now - pressStartMs >= TOUCH_PETTING_MIN_MS) {
                sendTouchEvent(TOUCH_EVENT_LONG); // EV_PETTING
                longPressDetected = true;
                tapCount          = 0;
            }
        }

        // 3. Fronte di discesa (rilascio del sensore)
        if (!currentState && previousState) {
            const uint32_t pressDuration = now - pressStartMs;

            if (!longPressDetected &&
                pressDuration >= TOUCH_MIN_TAP_MS &&
                pressDuration <  TOUCH_MAX_TAP_MS) {

                if (tapCount < 3) {
                    tapCount++;
                }
                lastReleaseMs = now;

                // Il triplo tap viene emesso immediatamente al terzo rilascio valido
                if (tapCount == 3) {
                    sendTouchEvent(TOUCH_EVENT_TRIPLE); // EV_TAP_3
                    tapCount = 0;
                }
            }
        }

        // 4. Scadenza finestra di inter-tap timeout (t_gap = 350 ms dal rilascio)
        if (!currentState &&
            tapCount > 0 &&
            (now - lastReleaseMs >= TOUCH_INTER_TAP_GAP_MS)) {

            if (tapCount == 1) {
                sendTouchEvent(TOUCH_EVENT_SINGLE); // EV_TAP_1
            } else if (tapCount == 2) {
                sendTouchEvent(TOUCH_EVENT_DOUBLE); // EV_TAP_2
            }
            tapCount = 0;
        }

        previousState = currentState;

        // Risveglio deterministico esattamente ogni T_2 = 25 ms
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}