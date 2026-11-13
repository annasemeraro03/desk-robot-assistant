#include "RobotState.h"

RobotState robotState = {
    .mood = MOOD_HAPPY,
    .mode = MODE_NORMAL,
    .movement = MOVEMENT_STOP,
    .currentPage = PAGE_EMOTION,
    .attention = 100.0f,
    .focusActive = false
};