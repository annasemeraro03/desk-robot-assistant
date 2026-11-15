#include "MoodTask.h"

#include <Arduino.h>

#include "../../config/Config.h"
#include "../../app/Robot.h"
#include "../../app/RobotEvents.h"
#include "../../app/RobotState.h"
#include "../../app/RobotConfig.h"

/**
 * @brief Effettua la transizione di stato azzerando il timer di permanenza
 *        nello stato corrente (t_state) e, se si entra in FELICE, il timer
 *        di inattivita' di 5 minuti (t_inactivity).
 */
static void enterState(RobotStateData *st, RobotFsmState nextState, const char *reason) {
    const RobotFsmState prevState = st->currentState;
    st->currentState   = nextState;
    st->stateElapsedMs = 0;

    if (nextState == STATE_FELICE) {
        st->inactivityElapsedMs = 0;
    }

    Serial.print("[FSM] ");
    Serial.print(robotStateToString(prevState));
    Serial.print(" ---> ");
    Serial.print(robotStateToString(nextState));
    Serial.print(" | Motivo: ");
    Serial.print(reason);
    if (st->focusSessionActive) {
        Serial.print(" | [Pomodoro Attivo - Residuo: ");
        Serial.print(st->focusRemainingMs / 1000UL);
        Serial.print("s]");
    }
    Serial.println();
}

/**
 * @brief Elabora un singolo evento prelevato dalla coda secondo la Tabella 3.3
 *        della specifica tecnica.
 */
static void processFsmEvent(RobotStateData *st, const RobotEvent *ev) {
    // ========================================================================
    // 1. PREEMPTION GLOBALE AD ALTA PRIORITA' (Specifica Sez. 3.2.B e Tab. 3.3)
    //    Da QUALSIASI STATO: se l'MPU6050 rileva sollevamento o sballottamento
    //    (EV_SHOCK_LIFT), il robot passa immediatamente in ARRABBIATA.
    //    Se era in corso una sessione CONCENTRATA (focusSessionActive == true),
    //    il countdown focusRemainingMs rimane CONGELATO al valore corrente!
    // ========================================================================
    if (ev->type == EVENT_MOTION && ev->id == MOTION_EVENT_SHAKE) {
        enterState(st, STATE_ARRABBIATA, "EV_SHOCK_LIFT (Preemption IMU)");
        return;
    }

    // ========================================================================
    // 2. TRANSIZIONI DIPENDENTI DALLO STATO CORRENTE (Tabella 3.3)
    // ========================================================================
    switch (st->currentState) {

        case STATE_ACCESA:
            // In ACCESA gli eventi touch/calendario vengono scartati;
            // l'uscita avviene dopo 2000 ms verso FELICE.
            break;

        case STATE_FELICE:
            // Qualsiasi interazione touch in FELICE (incluso 1 singolo tap che non
            // cambia stato) azzera il timer di inattivita' di 5 minuti (t_inactivity = 0)
            if (ev->type == EVENT_TOUCH) {
                st->inactivityElapsedMs = 0;

                if (ev->id == TOUCH_EVENT_LONG) {
                    // EV_PETTING (t_on > 800 ms) -> COCCOLATA
                    enterState(st, STATE_COCCOLATA, "EV_PETTING da FELICE");
                } else if (ev->id == TOUCH_EVENT_DOUBLE) {
                    // EV_TAP_2 (2 tap consecutivi) -> Avvia sessione Pomodoro in CONCENTRATA
                    st->focusSessionActive = true;
                    st->focusRemainingMs   = robotConfigGetFocusDurationMs();
                    enterState(st, STATE_CONCENTRATA, "EV_TAP_2 (Avvio Pomodoro)");
                } else if (ev->id == TOUCH_EVENT_TRIPLE) {
                    // EV_TAP_3 (3 tap consecutivi) -> INFORMAZIONI_METEO
                    enterState(st, STATE_INFORMAZIONI_METEO, "EV_TAP_3 (Richiesta Meteo)");
                } else if (ev->id == TOUCH_EVENT_SINGLE) {
                    // EV_TAP_1 in FELICE non cambia stato ma azzera t_inactivity
                    Serial.println("[FSM] EV_TAP_1 in FELICE -> Reset timer inattivita' (t_inactivity = 0)");
                }
            } else if (ev->type == EVENT_CALENDAR && ev->id == CALENDAR_EVENT_REMINDER) {
                // EV_CALENDAR_ALERT (-10 min) -> INFORMAZIONI_CALENDARIO
                enterState(st, STATE_INFORMAZIONI_CALENDARIO, "EV_CALENDAR_ALERT (-10 min)");
            }
            break;

        case STATE_ANNOIATA:
            // Da ANNOIATA si esce solo accarezzando il sensore (EV_PETTING -> COCCOLATA).
            // Gli altri eventi touch vengono scartati senza effetto.
            if (ev->type == EVENT_TOUCH && ev->id == TOUCH_EVENT_LONG) {
                enterState(st, STATE_COCCOLATA, "EV_PETTING da ANNOIATA");
            }
            break;

        case STATE_COCCOLATA:
            // Stato temporizzato di 4000 ms; eventuali tap durante la coccola sono ignorati.
            break;

        case STATE_CONCENTRATA:
            // Durante CONCENTRATA i tap sul sensore touch sono ignorati.
            // Se arriva EV_CALENDAR_ALERT (-10 min), avviene la preemption verso
            // INFORMAZIONI_CALENDARIO congelando focusRemainingMs (focusSessionActive = true).
            if (ev->type == EVENT_CALENDAR && ev->id == CALENDAR_EVENT_REMINDER) {
                enterState(st, STATE_INFORMAZIONI_CALENDARIO,
                           "EV_CALENDAR_ALERT (Preemption su CONCENTRATA, timer congelato)");
            }
            break;

        case STATE_VITTORIA:
            // Stato transitorio esplicito di 4000 ms; eventi touch ignorati.
            break;

        case STATE_INFORMAZIONI_CALENDARIO:
            // Resta in INFORMAZIONI_CALENDARIO finche' l'utente non preme 1 sola volta (EV_TAP_1).
            // All'uscita: se focusSessionActive == true ripristina CONCENTRATA dal tempo residuo,
            // altrimenti torna nello stato FELICE (Specifica Sez. 3.2.B e Tab. 3.3).
            if (ev->type == EVENT_TOUCH && ev->id == TOUCH_EVENT_SINGLE) {
                if (st->focusSessionActive) {
                    enterState(st, STATE_CONCENTRATA,
                               "EV_TAP_1 (Conferma Calendario -> Ripresa CONCENTRATA)");
                } else {
                    enterState(st, STATE_FELICE,
                               "EV_TAP_1 (Conferma Calendario -> Ritorno a FELICE)");
                }
            }
            break;

        case STATE_INFORMAZIONI_METEO:
            // Resta in INFORMAZIONI_METEO finche' l'utente non preme 1 sola volta (EV_TAP_1),
            // dopodiche' entra nello stato FELICE.
            if (ev->type == EVENT_TOUCH && ev->id == TOUCH_EVENT_SINGLE) {
                enterState(st, STATE_FELICE, "EV_TAP_1 (Uscita da INFO_METEO)");
            }
            break;

        case STATE_ARRABBIATA:
            // Resta in ARRABBIATA finche' non viene coccolato (EV_PETTING -> COCCOLATA per 4s).
            // Gli altri eventi touch vengono scartati senza effetto.
            if (ev->type == EVENT_TOUCH && ev->id == TOUCH_EVENT_LONG) {
                enterState(st, STATE_COCCOLATA, "EV_PETTING (Robot placato -> COCCOLATA)");
            }
            break;
    }
}

/**
 * @brief Aggiorna i timer deterministici della FSM ad ogni passo periodico T_4 = 50 ms
 *        e valuta le condizioni di guardia temporali (Specifica Sez. 3.2.B, 3.2.C e Tab. 3.3).
 */
static void updateFsmTimers(RobotStateData *st, uint32_t dtMs) {
    // Incrementa il tempo trascorso nello stato visivo corrente (t_state)
    st->stateElapsedMs += dtMs;

    switch (st->currentState) {

        case STATE_ACCESA:
            // Guardia: t_state >= 2000 ms -> transita automaticamente in FELICE
            if (st->stateElapsedMs >= TIME_ACCESA_MS) {
                enterState(st, STATE_FELICE, "Timeout 2000 ms fine avvio");
            }
            break;

        case STATE_FELICE:
            // SCOPING DEL TIMER DI INATTIVITA' (Specifica Sez. 3.2.C):
            // t_inactivity avanza ESCLUSIVAMENTE quando il robot si trova in FELICE.
            st->inactivityElapsedMs += dtMs;
            if (st->inactivityElapsedMs >= TIME_INACTIVITY_ANNOIATA_MS) {
                enterState(st, STATE_ANNOIATA, "Timeout inattivita' (t_inactivity >= 5 min)");
            }
            break;

        case STATE_COCCOLATA:
            // Guardia: t_state >= 4000 ms -> se una sessione Pomodoro era attiva ed e'
            // stata interrotta, ripristina CONCENTRATA dal tempo congelato; altrimenti FELICE.
            if (st->stateElapsedMs >= TIME_COCCOLATA_MS) {
                if (st->focusSessionActive) {
                    enterState(st, STATE_CONCENTRATA,
                               "Timeout 4000 ms fine COCCOLATA -> Ripresa CONCENTRATA");
                } else {
                    enterState(st, STATE_FELICE,
                               "Timeout 4000 ms fine COCCOLATA -> Ritorno a FELICE");
                }
            }
            break;

        case STATE_CONCENTRATA:
            // Il countdown focusRemainingMs avanza SOLO quando lo stato attivo e' CONCENTRATA:
            // durante ARRABBIATA, COCCOLATA o INFORMAZIONI_CALENDARIO resta congelato!
            if (st->focusRemainingMs > dtMs) {
                st->focusRemainingMs -= dtMs;
            } else {
                st->focusRemainingMs   = 0;
                st->focusSessionActive = false;
                enterState(st, STATE_VITTORIA, "Countdown Pomodoro completato (focusRemainingMs == 0)");
            }
            break;

        case STATE_VITTORIA:
            // Guardia: t_state >= 4000 ms -> termina animazione di vittoria e passa a FELICE
            if (st->stateElapsedMs >= TIME_VITTORIA_MS) {
                enterState(st, STATE_FELICE, "Timeout 4000 ms fine VITTORIA");
            }
            break;

        default:
            // In ANNOIATA, INFO_CALENDARIO, INFO_METEO e ARRABBIATA non ci sono
            // transizioni a tempo automatiche (si attende l'interazione dell'utente).
            break;
    }
}

/**
 * @brief Task Periodico Real-Time tau_4: Task_FSM_Core (Specifica Sez. 5.1)
 *        - Periodo:    T_4 = 50 ms (Deadline D_4 = 50 ms)
 *        - WCET:       C_4 = 2.0 ms
 *        - Bloccaggio: B_4 = 2.5 ms
 *        - Risposta:   R_4 = 10.5 ms <= D_4 = 50 ms
 *        - Priorita':  Media (3) su Core 1
 */
void moodTask(void *pvParameters) {
    (void)pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_FSM_PERIOD_MS);

    RobotStateData localState;
    robotStateGetSnapshot(&localState);

    Serial.println("[FSM] Stato iniziale: ACCESA (Hello)");

    for (;;) {
        // 1. Svuota ed elabora tutti gli eventi pendenti nella coda robotEventQueue
        RobotEvent incomingEvent;
        while (xQueueReceive(robotEventQueue, &incomingEvent, 0) == pdTRUE) {
            processFsmEvent(&localState, &incomingEvent);
        }

        // 2. Aggiorna i timer deterministici della FSM (passo costante dt = 50 ms)
        updateFsmTimers(&localState, TASK_FSM_PERIOD_MS);

        // 3. Pubblica lo stato aggiornato proteggendo la scrittura con stateMutex (<= 0.2 ms)
        robotStateSetSnapshot(&localState);

        // 4. Risveglio deterministico esattamente ogni T_4 = 50 ms
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}