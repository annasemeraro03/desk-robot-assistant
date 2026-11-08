
#include "MoodTask.h"

#include <Arduino.h>

#include "../../app/Robot.h"
#include "../../app/RobotEvents.h"
#include "../../app/RobotState.h"
#include "../../app/FSM/RobotFSM.h"

// FSM instance
static RobotFSM robotFSM;

/**
 * @brief Synchronize the public robot state with the FSM.
 *
 * @param fsm Current FSM instance.
 */
static void synchronizeRobotState(const RobotFSM *fsm) {

    robotState.movement = MOVEMENT_STOP;
    robotState.focusActive = fsm->focusSessionActive;

    switch (fsm->currentState) {

        case FSM_STATE_STARTUP:
        case FSM_STATE_HAPPY:
            robotState.mood = MOOD_HAPPY;
            robotState.mode = MODE_NORMAL;
            robotState.currentPage = PAGE_EMOTION;
            break;

        case FSM_STATE_BORED:
            robotState.mood = MOOD_BORED;
            robotState.mode = MODE_NORMAL;
            robotState.currentPage = PAGE_EMOTION;
            break;

        case FSM_STATE_CUDDLED:
            robotState.mood = MOOD_CHARMING;
            robotState.mode = MODE_NORMAL;
            robotState.currentPage = PAGE_EMOTION;
            break;

        case FSM_STATE_FOCUSED:
            robotState.mood = MOOD_FOCUS;
            robotState.mode = MODE_FOCUS;
            robotState.currentPage = PAGE_EMOTION;
            break;

        case FSM_STATE_CALENDAR_INFO:
            robotState.mood = MOOD_HAPPY;
            robotState.mode = MODE_NORMAL;
            robotState.currentPage = PAGE_CALENDAR;
            break;

        case FSM_STATE_WEATHER_INFO:
            robotState.mood = MOOD_HAPPY;
            robotState.mode = MODE_NORMAL;
            robotState.currentPage = PAGE_WEATHER;
            break;

        case FSM_STATE_ANGRY:
            robotState.mood = MOOD_ANGRY;
            robotState.mode = MODE_NORMAL;
            robotState.currentPage = PAGE_EMOTION;
            break;

        case FSM_STATE_VICTORY:
            robotState.mood = MOOD_VICTORY;
            robotState.mode = MODE_NORMAL;
            robotState.currentPage = PAGE_EMOTION;
            break;
    }
}

/**
 * @brief Convert incoming robot events into FSM events.
 *
 * @param event Event received from the robot event queue.
 */
static void processRobotEvent(const RobotEvent &event) {

    switch (event.type) {

        case EVENT_TOUCH:

            switch (event.id) {

                case TOUCH_EVENT_SINGLE:
                    robotFSMHandleEvent(&robotFSM, FSM_EVENT_SINGLE_TAP);
                    break;

                case TOUCH_EVENT_DOUBLE:
                    robotFSMHandleEvent(&robotFSM, FSM_EVENT_DOUBLE_TAP);
                    break;

                case TOUCH_EVENT_TRIPLE:
                    robotFSMHandleEvent(&robotFSM, FSM_EVENT_TRIPLE_TAP);
                    break;

                case TOUCH_EVENT_LONG:
                    robotFSMHandleEvent(&robotFSM, FSM_EVENT_PETTING);
                    break;

                default:
                    break;
            }

            break;

        case EVENT_MOTION:

            switch (event.id) {

                case MOTION_EVENT_SHAKE:
                case MOTION_EVENT_FALL:
                    robotFSMHandleEvent(&robotFSM, FSM_EVENT_SHAKE);
                    break;

                case MOTION_EVENT_EDGE_DETECTED:
                    robotFSMHandleEvent(&robotFSM, FSM_EVENT_EDGE_DETECTED);
                    break;

                default:
                    break;
            }

            break;

        case EVENT_CALENDAR:

            if (event.id == CALENDAR_EVENT_REMINDER) {
                robotFSMHandleEvent(&robotFSM, FSM_EVENT_CALENDAR_REMINDER);
            }

            break;

        case EVENT_FOCUS:

            switch (event.id) {

                case FOCUS_EVENT_STARTED:
                    robotFSMHandleEvent(&robotFSM, FSM_EVENT_FOCUS_STARTED);
                    break;

                case FOCUS_EVENT_FINISHED:
                    robotFSMHandleEvent(&robotFSM, FSM_EVENT_FOCUS_FINISHED);
                    break;

                default:
                    break;
            }

            break;

        case EVENT_POWER:
        case EVENT_NETWORK:
        default:
            break;
    }
}

/**
 * @brief FreeRTOS task responsible for robot mood management.
 *
 * Receives robot events, updates the FSM and synchronizes the
 * resulting state with the robot runtime data.
 *
 * @param pvParameters Task parameters (unused).
 */
void moodTask(void *pvParameters) {

    (void)pvParameters;

    RobotEvent event;

    robotFSMInit(&robotFSM);
    synchronizeRobotState(&robotFSM);

    for (;;) {

        if (xQueueReceive(robotEventQueue, &event, pdMS_TO_TICKS(50)) == pdPASS) {
            processRobotEvent(event);
        }

        robotFSMUpdate(&robotFSM);
        synchronizeRobotState(&robotFSM);
    }
}