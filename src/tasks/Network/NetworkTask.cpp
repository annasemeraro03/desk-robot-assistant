#include "NetworkTask.h"

#include <Arduino.h>

#include "../../app/Robot.h"
#include "../../app/RobotEvents.h"
#include "../../config/Config.h"

// Task execution period
static constexpr TickType_t NETWORK_TASK_PERIOD = pdMS_TO_TICKS(WEATHER_UPDATE_PERIOD_MS);

/**
 * @brief FreeRTOS task responsible for network communication.
 *
 * The task manages all network-related services, including Wi-Fi
 * connectivity, weather updates, calendar synchronization and
 * communication with the desktop application.
 *
 * The task runs periodically because network operations are relatively
 * slow and do not require real-time execution.
 *
 * @param pvParameters Task parameters (unused).
 */
void networkTask(void *pvParameters){
    // TODO:
    // Initialize Wi-Fi connection.

    for(;;){
        // TODO:
        // Check Wi-Fi connection status.

        // TODO:
        // Reconnect if the connection is lost.

        // TODO:
        // Synchronize system time using NTP.

        // TODO:
        // Download weather information.

        // TODO:
        // Synchronize calendar events.

        // TODO:
        // Detect desktop application connection
        // and send EVENT_PC_CONNECTED or
        // EVENT_PC_DISCONNECTED if needed.

        vTaskDelay(NETWORK_TASK_PERIOD);
    }
}