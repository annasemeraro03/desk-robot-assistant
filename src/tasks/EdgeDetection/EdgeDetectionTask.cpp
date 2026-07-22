#include "EdgeDetectionTask.h"

#include <Arduino.h>

#include "../../app/Robot.h"
#include "../../app/RobotEvents.h"
#include "../../drivers/MPU6050/MPU6050Driver.h"

// Task execution period
static constexpr TickType_t EDGE_TASK_PERIOD = pdMS_TO_TICKS(10);

/**
 * @brief FreeRTOS task responsible for motion and edge detection.
 *
 * The task periodically reads the MPU6050 sensor and analyzes the
 * acceleration and gyroscope measurements to detect significant
 * motion events, such as table edges or sudden shakes.
 *
 * When a motion event is detected, a corresponding RobotEvent is
 * sent to the global event queue so that other tasks can react
 * accordingly.
 *
 * Future versions will implement complementary or Kalman filters,
 * pitch/roll estimation and more robust edge detection algorithms.
 *
 * @param pvParameters Task parameters (unused).
 */
void edgeDetectionTask(void *pvParameters){
    MotionData motionData;

    for(;;){

        if(MPU6050Read(&motionData)){
            // TODO:
            // Estimate pitch and roll.

            // TODO:
            // Detect table edge.

            // TODO:
            // Detect sudden robot movement.

            /*
             * Example:
             *
             * RobotEvent event;
             * event.type = EVENT_MOTION;
             * event.code = MOTION_EVENT_SHAKE;
             *
             * xQueueSend(robotEventQueue, &event, 0);
             */
        }

        vTaskDelay(EDGE_TASK_PERIOD);
    }
}