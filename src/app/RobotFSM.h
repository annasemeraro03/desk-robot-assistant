#ifndef ROBOT_FSM_H
#define ROBOT_FSM_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief States of the robot finite-state machine.
 */
typedef enum : uint8_t {
    FSM_STATE_STARTUP,
    FSM_STATE_HAPPY,
    FSM_STATE_BORED,
    FSM_STATE_CUDDLED,
    FSM_STATE_FOCUSED,
    FSM_STATE_CALENDAR_INFO,
    FSM_STATE_WEATHER_INFO,
    FSM_STATE_ANGRY,
    FSM_STATE_VICTORY
} RobotFSMState;

/**
 * @brief Events processed by the robot FSM.
 */
typedef enum : uint8_t {
    FSM_EVENT_NONE,
    FSM_EVENT_SINGLE_TAP,
    FSM_EVENT_DOUBLE_TAP,
    FSM_EVENT_TRIPLE_TAP,
    FSM_EVENT_PETTING,
    FSM_EVENT_SHAKE,
    FSM_EVENT_EDGE_DETECTED,
    FSM_EVENT_CALENDAR_REMINDER,
    FSM_EVENT_FOCUS_STARTED,
    FSM_EVENT_FOCUS_FINISHED
} RobotFSMEvent;

/**
 * @brief Runtime data maintained by the robot FSM.
 */
typedef struct {
    RobotFSMState currentState;
    RobotFSMState previousState;

    // TODO: Consider adding an interruptedState field to preserve
    // the original state when ARRABBIATA interrupts another state.

    bool focusSessionActive;

    uint32_t focusRemainingMs;
    uint32_t focusStartMs;

    // TODO: Consider adding additional timer/context fields
    // if required by the interruption management logic.

    uint32_t stateStartMs;
    uint32_t lastInteractionMs;
} RobotFSM;

/**
 * @brief Initialize the robot finite-state machine.
 */
void robotFSMInit(RobotFSM *fsm);

/**
 * @brief Process an event received by the FSM.
 */
void robotFSMHandleEvent(RobotFSM *fsm, RobotFSMEvent event);

/**
 * @brief Update time-dependent state transitions.
 */
void robotFSMUpdate(RobotFSM *fsm);

#endif