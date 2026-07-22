#include "LocomotionTask.h"

#include <Arduino.h>

#include "../../app/Robot.h"
#include "../../app/RobotState.h"
#include "../../drivers/Motor/MotorDriver.h"

// Task execution period
static constexpr TickType_t LOCOMOTION_TASK_PERIOD = pdMS_TO_TICKS(50);

/**
 * @brief FreeRTOS task responsible for robot movement.
 *
 * The task periodically updates the robot locomotion according to the
 * current operating mode and movement command. It controls the motor
 * driver while ensuring that the robot remains stationary whenever
 * movement is disabled (e.g., Focus Mode, Sleep Mode or Power Saving Mode).
 *
 * Future versions will implement autonomous movement patterns,
 * obstacle avoidance and smooth acceleration profiles.
 *
 * @param pvParameters Task parameters (unused).
 */
void locomotionTask(void *pvParameters){

    for(;;){

        // Movement disabled
        if(robotState.mode != MODE_NORMAL){

            stopMotors();

            vTaskDelay(LOCOMOTION_TASK_PERIOD);
            continue;
        }

        // Execute the requested movement
        switch(robotState.movement){

            case MOVEMENT_STOP: {

                stopMotors();

                break;
            }

            case MOVEMENT_FORWARD: {

                // TODO:
                // Drive both motors forward.

                break;
            }

            case MOVEMENT_BACKWARD: {

                // TODO:
                // Drive both motors backward.

                break;
            }

            case MOVEMENT_LEFT: {

                // TODO:
                // Rotate left.

                break;
            }

            case MOVEMENT_RIGHT: {

                // TODO:
                // Rotate right.

                break;
            }

            case MOVEMENT_RANDOM: {

                // TODO:
                // Execute a random movement pattern.

                break;
            }
        }

        vTaskDelay(LOCOMOTION_TASK_PERIOD);
    }
}