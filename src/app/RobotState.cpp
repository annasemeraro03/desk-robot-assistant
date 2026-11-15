#include "RobotState.h"
#include "Robot.h"
#include "../config/Config.h"

// Istanza statica protetta da stateMutex
static RobotStateData sharedRobotState = {
    .currentState        = STATE_ACCESA,
    .stateElapsedMs      = 0,
    .inactivityElapsedMs = 0,
    .focusSessionActive  = false,
    .focusRemainingMs    = TIME_CONCENTRATA_DEFAULT_MS
};

void robotStateInit() {
    sharedRobotState.currentState        = STATE_ACCESA;
    sharedRobotState.stateElapsedMs      = 0;
    sharedRobotState.inactivityElapsedMs = 0;
    sharedRobotState.focusSessionActive  = false;
    sharedRobotState.focusRemainingMs    = TIME_CONCENTRATA_DEFAULT_MS;
}

void robotStateGetSnapshot(RobotStateData *outSnapshot) {
    if (outSnapshot == nullptr) {
        return;
    }

    // Sezione critica ultra-breve (semplice copia in RAM < 0.01 ms << C_cs,State = 0.2 ms)
    if (stateMutex != nullptr && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        *outSnapshot = sharedRobotState;
        xSemaphoreGive(stateMutex);
    } else {
        *outSnapshot = sharedRobotState;
    }
}

void robotStateSetSnapshot(const RobotStateData *newSnapshot) {
    if (newSnapshot == nullptr) {
        return;
    }

    // Sezione critica ultra-breve (< 0.01 ms << C_cs,State = 0.2 ms)
    if (stateMutex != nullptr && xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        sharedRobotState = *newSnapshot;
        xSemaphoreGive(stateMutex);
    } else {
        sharedRobotState = *newSnapshot;
    }
}

const char* robotStateToString(RobotFsmState state) {
    switch (state) {
        case STATE_ACCESA:                  return "ACCESA";
        case STATE_FELICE:                  return "FELICE";
        case STATE_ANNOIATA:                return "ANNOIATA";
        case STATE_COCCOLATA:               return "COCCOLATA";
        case STATE_CONCENTRATA:             return "CONCENTRATA";
        case STATE_VITTORIA:                return "VITTORIA";
        case STATE_INFORMAZIONI_CALENDARIO: return "INFO CALENDARIO";
        case STATE_INFORMAZIONI_METEO:      return "INFO METEO";
        case STATE_ARRABBIATA:              return "ARRABBIATA";
        default:                            return "SCONOSCIUTO";
    }
}