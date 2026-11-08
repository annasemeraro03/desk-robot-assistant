
#include "RobotFSM.h"

#include <Arduino.h>

// Default duration of a focus session (40 minutes).
static constexpr uint32_t FOCUS_DURATION_MS = 40UL * 60UL * 1000UL;

// Duration of the startup state (2 seconds).
static constexpr uint32_t STARTUP_DURATION_MS = 2000UL;

// Duration of the cuddled state (4 seconds).
static constexpr uint32_t CUDDLED_DURATION_MS = 4000UL;

// Duration of the victory state (4 seconds).
static constexpr uint32_t VICTORY_DURATION_MS = 4000UL;

// Inactivity threshold before entering the bored state (5 minutes).
static constexpr uint32_t BORED_TIMEOUT_MS = 5UL * 60UL * 1000UL;

/**
 * @brief Change the current FSM state and reset its timer.
 */
static void enterState(RobotFSM *fsm, RobotFSMState newState) {

    // TODO: Review the use of previousState.
    // It currently stores only the immediately preceding state.
    // Consider introducing a dedicated variable to preserve the
    // original state when multiple interruptions occur.

    fsm->previousState = fsm->currentState;
    fsm->currentState = newState;
    fsm->stateStartMs = millis();
}

/**
 * @brief Pause the focus timer and preserve the remaining time.
 */
static void pauseFocusTimer(RobotFSM *fsm) {
    if (!fsm->focusSessionActive) {
        return;
    }

    uint32_t now = millis();
    uint32_t elapsed = now - fsm->focusStartMs;

    if (elapsed >= fsm->focusRemainingMs) {
        fsm->focusRemainingMs = 0;
    } else {
        fsm->focusRemainingMs -= elapsed;
    }

    // TODO: Verify that the timer is paused exactly once
    // when leaving FSM_STATE_FOCUSED.
}

/**
 * @brief Resume the focus timer from the remaining time.
 */
static void resumeFocusTimer(RobotFSM *fsm) {
    fsm->focusStartMs = millis();

    // TODO: Verify that the timer is resumed only when a focus
    // session is active and there is remaining time.
}

/**
 * @brief Initialize the robot finite-state machine.
 */
void robotFSMInit(RobotFSM *fsm) {
    if (fsm == nullptr) {
        return;
    }

    fsm->currentState = FSM_STATE_STARTUP;
    fsm->previousState = FSM_STATE_STARTUP;

    fsm->focusSessionActive = false;
    fsm->focusRemainingMs = FOCUS_DURATION_MS;

    fsm->focusStartMs = millis();
    fsm->stateStartMs = millis();
    fsm->lastInteractionMs = millis();

    // TODO: Initialize any additional variables introduced
    // for interruption handling or focus timer management.
}

/**
 * @brief Process an event received by the FSM.
 */
void robotFSMHandleEvent(RobotFSM *fsm, RobotFSMEvent event) {
    if (fsm == nullptr) {
        return;
    }

    switch (event) {

        case FSM_EVENT_SINGLE_TAP:

            fsm->lastInteractionMs = millis();

            if (fsm->currentState == FSM_STATE_CALENDAR_INFO) {

                if (fsm->focusSessionActive) {
                    enterState(fsm, FSM_STATE_FOCUSED);
                    resumeFocusTimer(fsm);
                } else {
                    enterState(fsm, FSM_STATE_HAPPY);
                }

            } else if (fsm->currentState == FSM_STATE_WEATHER_INFO) {
                enterState(fsm, FSM_STATE_HAPPY);
            }

            // TODO: Verify whether a single tap should have any
            // additional effect in other states.

            break;

        case FSM_EVENT_DOUBLE_TAP:

            fsm->lastInteractionMs = millis();

            if (fsm->currentState == FSM_STATE_HAPPY) {

                fsm->focusSessionActive = true;
                fsm->focusRemainingMs = FOCUS_DURATION_MS;

                enterState(fsm, FSM_STATE_FOCUSED);
                resumeFocusTimer(fsm);
            }

            // TODO: Verify that a new focus session cannot
            // accidentally overwrite an existing one.

            break;

        case FSM_EVENT_TRIPLE_TAP:

            fsm->lastInteractionMs = millis();

            if (fsm->currentState == FSM_STATE_HAPPY) {
                enterState(fsm, FSM_STATE_WEATHER_INFO);
            }

            break;

        case FSM_EVENT_PETTING:

            fsm->lastInteractionMs = millis();

            if (fsm->currentState == FSM_STATE_HAPPY ||
                fsm->currentState == FSM_STATE_BORED ||
                fsm->currentState == FSM_STATE_ANGRY) {

                enterState(fsm, FSM_STATE_CUDDLED);
            }

            // TODO: Verify the expected behavior when petting
            // occurs during other states.

            break;

        case FSM_EVENT_SHAKE:

            // Shaking interrupts the current state.
            if (fsm->currentState != FSM_STATE_ANGRY) {

                // Preserve the remaining focus time before interruption.
                if (fsm->currentState == FSM_STATE_FOCUSED) {
                    pauseFocusTimer(fsm);
                }

                enterState(fsm, FSM_STATE_ANGRY);
            }

            // TODO: Verify that shaking correctly interrupts EVERY
            // state, including STARTUP, VICTORY, CALENDAR_INFO,
            // WEATHER_INFO and COCCOLATA.
            //
            // TODO: Preserve the interrupted context so that,
            // after petting and COCCOLATA, the FSM can return
            // to the appropriate state when required.

            break;

        case FSM_EVENT_CALENDAR_REMINDER:

            if (fsm->currentState == FSM_STATE_HAPPY ||
                fsm->currentState == FSM_STATE_FOCUSED) {

                // Pause the focus timer if concentration is interrupted.
                if (fsm->currentState == FSM_STATE_FOCUSED) {
                    pauseFocusTimer(fsm);
                }

                enterState(fsm, FSM_STATE_CALENDAR_INFO);
            }

            // TODO: Verify the behavior when a calendar reminder
            // arrives during another state.
            //
            // TODO: Prevent repeated calendar events from
            // overwriting an interruption already in progress.

            break;

        case FSM_EVENT_FOCUS_STARTED:

            if (fsm->currentState == FSM_STATE_HAPPY) {

                fsm->focusSessionActive = true;
                fsm->focusRemainingMs = FOCUS_DURATION_MS;

                enterState(fsm, FSM_STATE_FOCUSED);
                resumeFocusTimer(fsm);
            }

            // TODO: Check whether FOCUS_STARTED should be allowed
            // only from HAPPY or also from other appropriate states.

            break;

        case FSM_EVENT_FOCUS_FINISHED:

            // An external completion event is accepted only while
            // the focus timer is actually running.
            if (fsm->focusSessionActive &&
                fsm->currentState == FSM_STATE_FOCUSED) {

                fsm->focusSessionActive = false;
                fsm->focusRemainingMs = 0;

                enterState(fsm, FSM_STATE_VICTORY);
            }

            // TODO: Decide whether an external FOCUS_FINISHED event
            // should also terminate a paused focus session.

            break;

        case FSM_EVENT_EDGE_DETECTED:

            // TODO: Define the expected FSM behavior when an edge
            // is detected. The current implementation ignores it.

            break;

        case FSM_EVENT_NONE:
        default:
            break;
    }
}

/**
 * @brief Update time-dependent state transitions.
 */
void robotFSMUpdate(RobotFSM *fsm) {
    if (fsm == nullptr) {
        return;
    }

    uint32_t now = millis();

    switch (fsm->currentState) {

        case FSM_STATE_STARTUP:

            if (now - fsm->stateStartMs >= STARTUP_DURATION_MS) {
                enterState(fsm, FSM_STATE_HAPPY);
                fsm->lastInteractionMs = now;
            }

            // TODO: Verify how the startup state will be represented
            // by the graphics module (e.g. displaying "Hello").

            break;

        case FSM_STATE_HAPPY:

            if (now - fsm->lastInteractionMs >= BORED_TIMEOUT_MS) {
                enterState(fsm, FSM_STATE_BORED);
            }

            break;

        case FSM_STATE_CUDDLED:

            if (now - fsm->stateStartMs >= CUDDLED_DURATION_MS) {

                if (fsm->focusSessionActive) {

                    // Resume the interrupted focus session.
                    if (fsm->focusRemainingMs == 0) {
                        fsm->focusSessionActive = false;
                        enterState(fsm, FSM_STATE_VICTORY);
                    } else {
                        enterState(fsm, FSM_STATE_FOCUSED);
                        resumeFocusTimer(fsm);
                    }

                } else {
                    enterState(fsm, FSM_STATE_HAPPY);
                }
            }

            // TODO: Review the return transition after COCCOLATA.
            // It should consider the original interrupted context,
            // not only focusSessionActive.

            break;

        case FSM_STATE_FOCUSED: {

            uint32_t elapsed = now - fsm->focusStartMs;

            if (elapsed >= fsm->focusRemainingMs) {

                fsm->focusRemainingMs = 0;
                fsm->focusSessionActive = false;

                enterState(fsm, FSM_STATE_VICTORY);

            } else {

                // Update the remaining time without losing precision.
                fsm->focusRemainingMs -= elapsed;
                fsm->focusStartMs = now;
            }

            break;
        }

        case FSM_STATE_VICTORY:

            if (now - fsm->stateStartMs >= VICTORY_DURATION_MS) {
                enterState(fsm, FSM_STATE_HAPPY);
                fsm->lastInteractionMs = now;
            }

            // TODO: Verify whether VICTORY should be interruptible
            // by higher-priority events such as FSM_EVENT_SHAKE.

            break;

        default:

            // TODO: Review whether all states requiring timed
            // transitions are handled explicitly.

            break;
    }
}