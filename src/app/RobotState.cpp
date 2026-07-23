#include "RobotState.h"

RobotState robotState = {
    .mood = MOOD_HAPPY,
    .mode = MODE_NORMAL,
    .movement = MOVEMENT_STOP,
    .currentPage = PAGE_EMOTION,
    .energy = 100.0f,
    .attention = 100.0f,
    .focusActive = false
};