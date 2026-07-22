#include "GraphicsTask.h"

#include <Arduino.h>

#include "../../app/Robot.h"
#include "../../app/RobotState.h"
#include "../../drivers/OLED/OLEDDriver.h"
#include "../../config/Config.h"

// Task execution period
static constexpr TickType_t GRAPHICS_TASK_PERIOD = pdMS_TO_TICKS(DISPLAY_REFRESH_PERIOD_MS);

/**
 * @brief FreeRTOS task responsible for updating the OLED display.
 *
 * The task periodically refreshes the display according to the current
 * robot state. It renders the appropriate user interface page and
 * graphical elements, such as emotions, clock or weather information.
 *
 * Future versions will include animated facial expressions, eye
 * movements, battery indicators and additional interface pages.
 *
 * @param pvParameters Task parameters (unused).
 */
void graphicsTask(void *pvParameters){
    for(;;){
        oledClear();        // Clear display buffer

        // Draw current page
        switch(robotState.currentPage){

            case PAGE_EMOTION: {
                oledDrawFace(robotState.mood);

                break;
            }

            case PAGE_CLOCK: {
                // TODO:
                // Display current time.

                oledDrawText(0, 0, "Clock");

                break;
            }

            case PAGE_WEATHER: {
                // TODO:
                // Display weather information.

                oledDrawText(0, 0, "Weather");

                break;
            }
        }

        oledUpdate();       // Update OLED display

        vTaskDelay(GRAPHICS_TASK_PERIOD);
    }
}