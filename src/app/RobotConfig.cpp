#include "RobotConfig.h"
#include "Robot.h"
#include "../config/Config.h"

RobotConfig robotConfig = {
    .focusDuration      = 40,
    .powerSavingEnabled = false,
    .city               = "Modena",
    .weatherSummary     = "22C Sereno",
    .nextCalendarEvent  = "Esame RTES -10m"
};

uint32_t robotConfigGetFocusDurationMs() {
    uint8_t durationMin = 40;

    if (stateMutex != nullptr && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        durationMin = robotConfig.focusDuration;
        xSemaphoreGive(stateMutex);
    } else {
        durationMin = robotConfig.focusDuration;
    }

#if DEBUG_SHORT_TIMERS
    // In modalita' test a banco, se il valore e' quello di default (40 min),
    // restituisce 30 secondi per testare comodamente CONCENTRATA -> VITTORIA -> FELICE.
    if (durationMin == 40) {
        return TIME_CONCENTRATA_DEFAULT_MS;
    }
#endif

    return (uint32_t)durationMin * 60UL * 1000UL;
}