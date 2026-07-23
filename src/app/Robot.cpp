#include "Robot.h"

#include <Arduino.h>

// Tasks
#include "../tasks/EdgeDetection/EdgeDetectionTask.h"
#include "../tasks/Touch/TouchTask.h"
#include "../tasks/Locomotion/LocomotionTask.h"
#include "../tasks/Graphics/GraphicsTask.h"
#include "../tasks/Mood/MoodTask.h"
#include "../tasks/Network/NetworkTask.h"
#include "../tasks/Power/PowerTask.h"

// Robot Events
#include "RobotEvents.h"

// Drivers
#include "../drivers/MPU6050/MPU6050Driver.h"
#include "../drivers/OLED/OLEDDriver.h"
#include "../drivers/Touch/TouchDriver.h"
#include "../drivers/Motor/MotorDriver.h"
#include "../drivers/Battery/BatteryDriver.h"

// Diagnostics
#include "../diagnostics/HardwareDiagnostics/HardwareDiagnostics.h"

#define DIAGNOSTICS true

// Synchronization object
QueueHandle_t robotEventQueue;
SemaphoreHandle_t i2cMutex;
SemaphoreHandle_t locomotionMutex;

void robotInit(){
    Serial.println("Initializing robot...");

    #if DIAGNOSTICS
        runHardwareDiagnostics();
    #endif

    /*
     * Initialize hardware drivers
     */
    batteryInit();
    touchInit();
    motorDriverInit();
    oledInit();
    MPU6050Init();

    /*
     * Create FreeRTOS synchronization objects
     */
    robotEventQueue = xQueueCreate(10, sizeof(RobotEvent));

    i2cMutex = xSemaphoreCreateMutex();
    locomotionMutex = xSemaphoreCreateMutex();

    /*
     * Create FreeRTOS tasks
     */
    xTaskCreatePinnedToCore(edgeDetectionTask, "EdgeDetection", 2048, NULL, 4, NULL, 0);
    xTaskCreatePinnedToCore(touchTask,"Touch", 2048, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(locomotionTask, "Locomotion", 2048, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(graphicsTask, "Graphics", 4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(moodTask, "Mood", 2048, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(networkTask, "Network", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(powerTask, "Power", 2048, NULL, 1, NULL, 1);

    Serial.println("Robot initialization completed.");
}