
#include "TouchTask.h"

#include <Arduino.h>

#include "../../drivers/Touch/TouchDriver.h"
#include "../../app/RobotEvents.h"
#include "../../app/Robot.h"

// Polling period
static constexpr TickType_t TOUCH_PERIOD = pdMS_TO_TICKS(20);

// Touch classification timings
static constexpr uint32_t MIN_TAP_DURATION_MS = 50;
static constexpr uint32_t MAX_TAP_DURATION_MS = 400;
static constexpr uint32_t PETTING_DURATION_MS = 800;
static constexpr uint32_t TAP_GROUP_WINDOW_MS = 350;

/**
 * @brief Send a touch event to the robot event queue.
 *
 * @param eventId Touch event identifier.
 */
static void sendTouchEvent(TouchEvent eventId) {
    RobotEvent event;

    event.type = EVENT_TOUCH;
    event.id = eventId;

    xQueueSend(robotEventQueue, &event, 0);
}

/**
 * @brief FreeRTOS task that classifies touch interactions.
 *
 * Detects single, double and triple taps, as well as long
 * presses used to represent petting interactions.
 *
 * @param pvParameters Task parameters (unused).
 */
void touchTask(void *pvParameters) {

    (void)pvParameters;

    bool previousState = false;
    bool longPressDetected = false;

    uint32_t pressStartMs = 0;
    uint32_t lastReleaseMs = 0;

    uint8_t tapCount = 0;

    for (;;) {

        bool currentState = touchIsPressed();
        uint32_t now = millis();

        // Detect touch rising edge
        if (currentState && !previousState) {
            pressStartMs = now;
            longPressDetected = false;
        }

        // Detect a long press (petting)
        if (currentState && !longPressDetected) {

            if (now - pressStartMs >= PETTING_DURATION_MS) {

                sendTouchEvent(TOUCH_EVENT_LONG);

                longPressDetected = true;
                tapCount = 0;
            }
        }

        // Detect touch release
        if (!currentState && previousState) {

            uint32_t pressDuration = now - pressStartMs;

            if (!longPressDetected &&
                pressDuration >= MIN_TAP_DURATION_MS &&
                pressDuration <= MAX_TAP_DURATION_MS) {

                if (tapCount < 3) {
                    tapCount++;
                }

                lastReleaseMs = now;

                // Three taps are immediately recognized
                if (tapCount == 3) {
                    sendTouchEvent(TOUCH_EVENT_TRIPLE);
                    tapCount = 0;
                }
            }
        }

        // Classify pending taps after the grouping window
        if (!currentState &&
            tapCount > 0 &&
            now - lastReleaseMs >= TAP_GROUP_WINDOW_MS) {

            if (tapCount == 1) {
                sendTouchEvent(TOUCH_EVENT_SINGLE);
            } else if (tapCount == 2) {
                sendTouchEvent(TOUCH_EVENT_DOUBLE);
            }

            tapCount = 0;
        }

        previousState = currentState;

        vTaskDelay(TOUCH_PERIOD);
    }
}