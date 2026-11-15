#ifndef ROBOT_STATE_H
#define ROBOT_STATE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief I 9 stati della Macchina a Stati Finiti deterministica (Specifica Sez. 3.1 e 3.3):
 *        8 stati base + 1 stato transitorio esplicito (STATE_VITTORIA).
 */
typedef enum : uint8_t {
    STATE_ACCESA = 0,               // Stato iniziale all'avvio ("Hello" per 2s)
    STATE_FELICE,                   // Espressione contenta, inattivita' monitorata (5 min)
    STATE_ANNOIATA,                 // Dopo 5 min senza interazioni in FELICE
    STATE_COCCOLATA,                // Carezza > 800ms (dura 4s, poi riprende CONCENTRATA o FELICE)
    STATE_CONCENTRATA,              // Sessione Pomodoro (default 40 min)
    STATE_VITTORIA,                 // Stato transitorio esplicito di fine concentrazione (4s)
    STATE_INFORMAZIONI_CALENDARIO,  // Preavviso impegno a -10 min (esce con 1 tap)
    STATE_INFORMAZIONI_METEO,       // Attivato con 3 tap da FELICE (esce con 1 tap)
    STATE_ARRABBIATA                // Preemption immediata su sollevamento/sballottamento IMU
} RobotFsmState;

/**
 * @brief Struttura dello stato condiviso del robot (Specifica Sez. 3.2.B e 5.2).
 *        Protetta da stateMutex (sezione critica massima C_cs,State <= 0.2 ms).
 */
typedef struct {
    RobotFsmState currentState;     // Stato attivo corrente della FSM
    uint32_t stateElapsedMs;        // Tempo trascorso nello stato corrente (t_state)
    uint32_t inactivityElapsedMs;   // Timer di inattivita' (t_inactivity, attivo solo in FELICE)
    bool focusSessionActive;        // Flag History State per preservare CONCENTRATA in preemption
    uint32_t focusRemainingMs;      // Tempo residuo Pomodoro (congelato durante preemption)
} RobotStateData;

/**
 * @brief Inizializza lo stato condiviso ai valori di avvio (STATE_ACCESA).
 */
void robotStateInit();

/**
 * @brief Legge una copia thread-safe dello stato corrente acquisendo stateMutex.
 *        Garantisce durata della sezione critica << 0.2 ms (Specifica Sez. 5.2).
 *
 * @param outSnapshot Puntatore alla struttura dove copiare lo stato.
 */
void robotStateGetSnapshot(RobotStateData *outSnapshot);

/**
 * @brief Aggiorna in modo thread-safe lo stato condiviso acquisendo stateMutex.
 *        Invocato dal task della FSM (tau_4: MoodTask).
 *
 * @param newSnapshot Puntatore ai nuovi dati di stato da pubblicare.
 */
void robotStateSetSnapshot(const RobotStateData *newSnapshot);

/**
 * @brief Restituisce il nome testuale dello stato per display OLED e log seriale.
 */
const char* robotStateToString(RobotFsmState state);

#endif