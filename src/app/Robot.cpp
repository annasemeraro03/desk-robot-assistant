#include "Robot.h"

#include <Arduino.h>
#include <Wire.h>

#include "../config/Pins.h"
#include "../config/Config.h"

// Stato e Configurazione
#include "RobotEvents.h"
#include "RobotState.h"
#include "RobotConfig.h"

// I 6 Task Periodici Real-Time (Specifica Sez. 5.1)
#include "../tasks/EdgeDetection/EdgeDetectionTask.h"
#include "../tasks/Touch/TouchTask.h"
#include "../tasks/Locomotion/LocomotionTask.h"
#include "../tasks/Mood/MoodTask.h"
#include "../tasks/Graphics/GraphicsTask.h"
#include "../tasks/Network/NetworkTask.h"

// Driver Hardware
#include "../drivers/MPU6050/MPU6050Driver.h"
#include "../drivers/OLED/OLEDDriver.h"
#include "../drivers/Touch/TouchDriver.h"
#include "../drivers/Motor/MotorDriver.h"

// Oggetti FreeRTOS globali
QueueHandle_t robotEventQueue    = nullptr;
SemaphoreHandle_t i2cMutex       = nullptr;
SemaphoreHandle_t stateMutex     = nullptr;
SemaphoreHandle_t locomotionMutex = nullptr;

void robotInit() {
    Serial.println("==================================================");
    Serial.println("[ROBOT EMOTIVO] Avvio Inizializzazione Real-Time");
    Serial.println("==================================================");

    // 1. Inizializza lo stato iniziale della FSM (STATE_ACCESA)
    robotStateInit();

    // 2. Crea le primitive di sincronizzazione FreeRTOS PRIMA di avviare i task
    //    xSemaphoreCreateMutex() implementa nativamente il Priority Inheritance Protocol (PIP)
    robotEventQueue = xQueueCreate(16, sizeof(RobotEvent));
    i2cMutex        = xSemaphoreCreateMutex();
    stateMutex      = xSemaphoreCreateMutex();
    locomotionMutex = xSemaphoreCreateMutex();

    // 3. Inizializza il bus I2C condiviso (D4 = SDA, D5 = SCL) in Fast Mode (400 kHz)
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(I2C_FAST_MODE_HZ);

    // 4. Inizializza i driver hardware
    touchInit();
    motorDriverInit();
    oledInit();
    MPU6050Init();
    // Ripristina 400 kHz nel caso una libreria abbia reimpostato il clock I2C
    Wire.setClock(I2C_FAST_MODE_HZ);

    // 5. Partizionamento Dual-Core (Specifica Sez. 4.1 e Tabella 5.1):
    //    Tutti i 6 task periodici sono fissati (pinned) sul Core 1 (CORE_RT_APP_CPU = 1)
    //    con priorita' assegnate secondo Rate Monotonic (da 6 a 1).

    // tau_1: Task_IMU_Safety (T1 = 20ms, D1 = 20ms, C1 = 2.0ms, Priorita' RM = 6 Massima)
    xTaskCreatePinnedToCore(
        edgeDetectionTask,
        "Task_IMU_Safety",
        4096,
        nullptr,
        PRIO_TASK_IMU_SAFETY,
        nullptr,
        CORE_RT_APP_CPU
    );

    // tau_2: Task_Touch_Input (T2 = 25ms, D2 = 25ms, C2 = 1.0ms, Priorita' RM = 5 Alta)
    xTaskCreatePinnedToCore(
        touchTask,
        "Task_Touch_Input",
        3072,
        nullptr,
        PRIO_TASK_TOUCH_INPUT,
        nullptr,
        CORE_RT_APP_CPU
    );

    // tau_3: Task_Motor_Ctrl (T3 = 50ms, D3 = 50ms, C3 = 3.0ms, Priorita' RM = 4 Media-Alta)
    xTaskCreatePinnedToCore(
        locomotionTask,
        "Task_Motor_Ctrl",
        3072,
        nullptr,
        PRIO_TASK_MOTOR_CTRL,
        nullptr,
        CORE_RT_APP_CPU
    );

    // tau_4: Task_FSM_Core (T4 = 50ms, D4 = 50ms, C4 = 2.0ms, Priorita' RM = 3 Media)
    xTaskCreatePinnedToCore(
        moodTask,
        "Task_FSM_Core",
        4096,
        nullptr,
        PRIO_TASK_FSM_CORE,
        nullptr,
        CORE_RT_APP_CPU
    );

    // tau_5: Task_OLED_UI (T5 = 100ms, D5 = 100ms, C5 = 18.0ms, Priorita' RM = 2 Bassa)
    xTaskCreatePinnedToCore(
        graphicsTask,
        "Task_OLED_UI",
        4096,
        nullptr,
        PRIO_TASK_OLED_UI,
        nullptr,
        CORE_RT_APP_CPU
    );

    // tau_6: Task_Sync_Cal (T6 = 500ms, D6 = 500ms, C6 = 15.0ms, Priorita' RM = 1 Minima)
    xTaskCreatePinnedToCore(
        networkTask,
        "Task_Sync_Cal",
        4096,
        nullptr,
        PRIO_TASK_SYNC_CAL,
        nullptr,
        CORE_RT_APP_CPU
    );

    Serial.println("[ROBOT EMOTIVO] Tutti i 6 task RM avviati su Core 1.");
    Serial.println("[COMANDI SERIALI DI TEST]:");
    Serial.println("  'c' -> Inietta EV_CALENDAR_ALERT (-10 min impegno)");
    Serial.println("  's' -> Simula EV_SHOCK_LIFT (sballottamento -> ARRABBIATA)");
    Serial.println("  '1' -> Simula EV_TAP_1 (1 tap)");
    Serial.println("  '2' -> Simula EV_TAP_2 (2 tap -> CONCENTRATA)");
    Serial.println("  '3' -> Simula EV_TAP_3 (3 tap -> INFO METEO)");
    Serial.println("  'p' -> Simula EV_PETTING (carezza > 800ms -> COCCOLATA)");
}