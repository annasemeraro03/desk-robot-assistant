#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Parametri del robot configurabili dall'utente tramite GUI su PC.
 */
typedef struct {
    uint8_t focusDuration;          // Durata CONCENTRATA in minuti (default 40)
    bool powerSavingEnabled;        // Modalita' risparmio energetico
    const char* city;               // Localita' meteo
    const char* weatherSummary;     // Stringa descrittiva meteo
    const char* nextCalendarEvent;  // Nome del prossimo impegno a -10 min
} RobotConfig;

extern RobotConfig robotConfig;

/**
 * @brief Restituisce la durata della sessione di concentrazione in millisecondi.
 *        Se DEBUG_SHORT_TIMERS == 1 e non e' stata modificata da PC, usa il timer breve.
 */
uint32_t robotConfigGetFocusDurationMs();

#endif