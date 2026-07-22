#include "TouchTask.h"

#include <Arduino.h>

#include "../../drivers/Touch/TouchDriver.h"
#include "../../app/RobotEvents.h"
#include "../../app/Robot.h"

// Polling period
static constexpr TickType_t TOUCH_PERIOD = pdMS_TO_TICKS(20);

/**
 * @brief FreeRTOS task that handles the touch sensor.
 *
 * The task periodically reads the touch sensor, detects user
 * interactions (single touch, double touch and long touch)
 * and sends the corresponding events to the robot event queue.
 *
 * @param pvParameters Pointer to task parameters (unused).
 */
void touchTask(void *pvParameters){
    bool previousState = false;

    for(;;){
        bool currentState = touchIsPressed();

        // Detect touch rising edge
        if(currentState && !previousState){
            RobotEvent event;

            event.type = EVENT_TOUCH;
            event.id = TOUCH_EVENT_SINGLE;

            xQueueSend(robotEventQueue, &event, 0);
        }

        previousState = currentState;

        vTaskDelay(TOUCH_PERIOD);
    }
}