#include <Arduino.h>
#include <Wire.h>

#include "config/Pins.h"
#include "config/Config.h"
#include "app/Robot.h"

void setup() {
    // 1. Avvia la comunicazione seriale verso il PC
    Serial.begin(SERIAL_BAUDRATE);
    delay(1500); // Attende la stabilizzazione della porta USB CDC nativa su ESP32-S3

    // 2. Avvia l'architettura Real-Time del Robot Emotivo
    robotInit();
}

void loop() {
    // Il task loopTask di Arduino viene creato di default sul Core 1 (APP_CPU) con priorita' 1.
    // Lo eliminiamo immediatamente affinche' sul Core 1 vengano schedulati ESCLUSIVAMENTE
    // i 6 task periodici Real-Time del modello Rate Monotonic (Specifica Sez. 4.1 e 5.1).
    vTaskDelete(nullptr);
}