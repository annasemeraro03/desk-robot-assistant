/*#include <Arduino.h>
// TO DO: Includere le librerie hardware specifiche: 
// - U8g2 o Adafruit_SH110X per il display OLED
// - Adafruit_MPU6050 o I2Cdev per il giroscopio
// - WiFi.h e NTPClient.h per rete e orologio

// --- DEFINIZIONE PIN HARDWARE ---
#define PIN_TOUCH 4
// TO DO: Definire i pin PWM per i due driver DRV8833 (4 canali necessari per 4 motori/trazione 4WD)
// TO DO: Definire i pin I2C personalizzati (SDA, SCL) se diversi dai default dello XIAO
// TO DO: Definire il pin ADC collegato al partitore di tensione per la batteria

// --- MACCHINE A STATI E STRUTTURE DATI ---
typedef enum { MOOD_HAPPY, MOOD_DIZZY, MOOD_ANGRY, MOOD_SURPRISED, MOOD_CHARMING, MOOD_FOCUS, MOOD_SLEEPY, MOOD_BORED } RobotMood;
typedef enum { PAGE_EMO, PAGE_CLOCK, PAGE_WEATHER } UIPage;
typedef enum { TOUCH_NONE, TOUCH_SINGLE, TOUCH_DOUBLE, TOUCH_LONG } TouchEvent;

// Variabili di stato globali (lette da più task)
volatile RobotMood currentMood = MOOD_HAPPY;
volatile UIPage currentPage = PAGE_EMO;
volatile float systemAttention = 100.0f;
volatile float systemEnergy = 100.0f;

// --- PRIMITIVE DI SINCRONIZZAZIONE (FreeRTOS) ---
QueueHandle_t touchQueue;
QueueHandle_t imuEventQueue;
SemaphoreHandle_t i2cMutex;        // Protegge il bus I2C (OLED e MPU6050 non devono accavallarsi)
SemaphoreHandle_t locomotionMutex; // Sincronizzazione critica per bloccare i motori (emergenze/Focus Mode)

// --- PROTOTIPI DEI TASK ---
void vTaskEdgeDetection(void *pvParameters);
void vTaskTouchInterface(void *pvParameters);
void vTaskLocomotion(void *pvParameters);
void vTaskGraphicsEngine(void *pvParameters);
void vTaskMoodEngine(void *pvParameters);
void vTaskNetwork(void *pvParameters);
void vTaskPower(void *pvParameters);

// --- INTERRUPT SERVICE ROUTINES (ISR) ---
void IRAM_ATTR touchISR() {
    // TO DO: Gestire l'interrupt hardware generato dal TTP223.
    // TO DO: Risvegliare il task vTaskTouchInterface utilizzando un semaforo binario 
    // con la funzione xSemaphoreGiveFromISR(), garantendo la minima latenza possibile.
}

// ==========================================
//                 SETUP
// ==========================================
void setup() {
    Serial.begin(115200);

    // TO DO: Configurare pinMode per input (touch) e output (motori).
    // TO DO: Configurare il sottosistema LEDC dell'ESP32 per la generazione PWM (DRV8833).
    // TO DO: Inizializzare Wire.begin() e chiamare i setup() di OLED e MPU6050.

    // 1. Creazione delle primitive IPC
    touchQueue = xQueueCreate(10, sizeof(TouchEvent));
    imuEventQueue = xQueueCreate(5, sizeof(uint8_t)); // Es: 1=Bordo Rilevato, 2=Scossone
    i2cMutex = xSemaphoreCreateMutex();
    locomotionMutex = xSemaphoreCreateMutex();

    // 2. Collegamento Interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_TOUCH), touchISR, RISING);

    // 3. Allocazione Task sui Core
    // Core 0 (PRO_CPU): Gestisce task ad alta priorità e tolleranza zero alla latenza.
    xTaskCreatePinnedToCore(vTaskEdgeDetection,  "EdgeIMU", 2048, NULL, 4, NULL, 0);
    xTaskCreatePinnedToCore(vTaskTouchInterface, "TouchUI", 2048, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(vTaskLocomotion,     "Motors",  2048, NULL, 3, NULL, 0);

    // Core 1 (APP_CPU): Gestisce rendering, logica di stato e chiamate bloccanti di rete.
    xTaskCreatePinnedToCore(vTaskGraphicsEngine, "Graphics", 4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(vTaskMoodEngine,     "Moods",    2048, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(vTaskNetwork,        "Network",  4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(vTaskPower,          "Power",    1024, NULL, 1, NULL, 1);
}

void loop() {
    // Il loop base di Arduino viene soppresso eliminando il suo task.
    // Tutto il flusso di esecuzione è ora interamente governato dallo scheduler FreeRTOS.
    vTaskDelete(NULL);
}

// ==========================================
//          IMPLEMENTAZIONE TASK
// ==========================================

void vTaskEdgeDetection(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // Loop rigoroso a 100Hz

    for(;;) {
        // TO DO: Acquisire i2cMutex con xSemaphoreTake().
        // TO DO: Leggere registri accelerometro e giroscopio.
        // TO DO: Rilasciare i2cMutex con xSemaphoreGive().

        // TO DO: Implementare filtro (es. Complementare o Kalman) per pitch e roll.
        // TO DO: Se l'accelerazione supera una soglia critica (robot scosso) 
        //        o il pitch indica il vuoto (bordo tavolo):
        //        1. Azzerare istantaneamente il PWM dei DRV8833 (Hard Real-Time).
        //        2. Inviare l'evento di allarme alla imuEventQueue.

        vTaskDelayUntil(&xLastWakeTime, xFrequency); // Ritardo assoluto per evitare drift temporale
    }
}

void vTaskTouchInterface(void *pvParameters) {
    for(;;) {
        // TO DO: Attendere in stato di blocco (portMAX_DELAY) il semaforo sbloccato dalla touchISR.
        // TO DO: Una volta sbloccato, implementare una logica di debouncing software e misurare 
        //        il tempo di pressione (millis o Timer RTOS).
        // TO DO: Classificare il tocco (Singolo = cambia Mood, Doppio = Start/Stop Focus Timer, 
        //        Lungo = cambia pagina).
        // TO DO: Inserire il TouchEvent rilevato nella touchQueue tramite xQueueSend().
    }
}

void vTaskLocomotion(void *pvParameters) {
    for(;;) {
        // TO DO: Controllare se locomotionMutex è disponibile. Se non lo è (es. Focus Mode 
        //        o batteria scarica), mantenere i motori fermi e bypassare il ciclo.
        
        // TO DO: Implementare uno switch-case basato sul pattern di locomozione corrente.
        // TO DO: Calcolare e aggiornare i duty cycle per i 4 canali PWM dei DRV8833,
        //        garantendo curve di accelerazione fluide per evitare picchi di corrente.
        
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void vTaskGraphicsEngine(void *pvParameters) {
    for(;;) {
        // TO DO: Acquisire i2cMutex (Attendere finché disponibile).
        // TO DO: Valutare currentPage.
        // - Se PAGE_EMO: Calcolare la cinematica procedurale degli occhi. Applicare inerzia vettoriale 
        //   derivata dall'ultima lettura nota dell'MPU6050. Modificare le pupille in base a currentMood.
        // - Se PAGE_CLOCK / PAGE_WEATHER: Renderizzare i testi e i dati provenienti dalle globali.
        
        // TO DO: Eseguire la push del buffer video (display.display()).
        // TO DO: Rilasciare i2cMutex.
        
        vTaskDelay(pdMS_TO_TICKS(33)); // Target fluido a ~30 FPS
    }
}

void vTaskMoodEngine(void *pvParameters) {
    for(;;) {
        // TO DO: Eseguire xQueueReceive() su touchQueue e imuEventQueue (non bloccanti o con timeout basso).
        // TO DO: Gestire le logiche comportamentali:
        // - Se riceve evento "scossone" -> currentMood = MOOD_DIZZY, poi schedulare timer per MOOD_ANGRY.
        // - Decrementare dolcemente systemEnergy se i motori sono attivi.
        // - Decrementare systemAttention se le code degli input sono vuote a lungo.
        // - Se systemEnergy < soglia logica -> currentMood = MOOD_SLEEPY.
        
        vTaskDelay(pdMS_TO_TICKS(100)); // Logica soft-real time a 10Hz
    }
}

void vTaskNetwork(void *pvParameters) {
    // TO DO: Connessione WiFi iniziale (tentare riconnessione automatica se persa).
    for(;;) {
        // TO DO: Richiesta HTTP GET alle API di OpenWeather.
        // TO DO: Parsing del JSON (usando ad es. ArduinoJson) e aggiornamento dati meteo.
        // TO DO: Richiesta NTP per l'aggiornamento dell'orologio locale.
        
        // Trattandosi di operazioni lente e soggette a timeout esterni, questo task 
        // deve restare nel Core 1 e avere priorità bassa.
        vTaskDelay(pdMS_TO_TICKS(60000 * 5)); // Aggiornamento ogni 5 minuti
    }
}

void vTaskPower(void *pvParameters) {
    for(;;) {
        // TO DO: Leggere analogRead dal pin connesso al partitore della batteria LiPo (1000mAh).
        // TO DO: Applicare un filtro passa-basso ai campioni letti.
        // TO DO: Se tensione < 3.4V (Soglia di salvaguardia):
        //        1. Acquisire forzatamente locomotionMutex per impedire qualsiasi movimento.
        //        2. Arrestare la radio WiFi (esp_wifi_stop()).
        //        3. Forzare currentMood su MOOD_SLEEPY e disattivare backlight o grafiche pesanti.
        
        vTaskDelay(pdMS_TO_TICKS(5000)); // Campionamento ogni 5 secondi
    }
}*/