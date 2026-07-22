#include "PowerTask.h"

#include <Arduino.h>

#include "../../drivers/Battery/BatteryDriver.h"
#include "../../app/Robot.h"
#include "../../app/RobotEvents.h"
#include "../../config/Config.h"

// Task execution period
static constexpr TickType_t POWER_TASK_PERIOD = pdMS_TO_TICKS(BATTERY_CHECK_PERIOD_MS);

/**
 * @brief FreeRTOS task responsible for battery monitoring.
 *
 * The task periodically reads the battery status through the
 * BatteryDriver. If the battery voltage falls below the configured
 * threshold, a low-battery event is sent to the global event queue.
 *
 * Future versions of this task will also manage power-saving mode,
 * peripheral shutdown and battery charging detection.
 *
 * @param pvParameters Task parameters (unused).
 */
void powerTask(void *pvParameters){
    for(;;){
        // Check battery level
        if(batteryIsLow()){
            RobotEvent event;

            event.type = EVENT_POWER;
            event.id = POWER_EVENT_LOW_BATTERY;

            xQueueSend(robotEventQueue, &event, 0);
        }

        vTaskDelay(POWER_TASK_PERIOD);
    }
}