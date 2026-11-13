#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

#include <Arduino.h>
#include "../config/Pins.h"

namespace MotorTest {

    struct WheelChannel {
        uint8_t pinA;
        uint8_t pinB;
        const char* label;
    };

    // Mappatura dei 4 motori secondo la tabella pinout della specifica (Sez. 2.2)
    constexpr WheelChannel WHEELS[4] = {
        {PIN_MOTOR_1A, PIN_MOTOR_1B, "Motore 1 (DRV8833 #1 Sup - D0/D1)"},
        {PIN_MOTOR_2A, PIN_MOTOR_2B, "Motore 2 (DRV8833 #1 Sup - D2/D3)"},
        {PIN_MOTOR_3A, PIN_MOTOR_3B, "Motore 3 (DRV8833 #2 Inf - D10/D9)"},
        {PIN_MOTOR_4A, PIN_MOTOR_4B, "Motore 4 (DRV8833 #2 Inf - D8/D7)"}
    };

    inline void init() {
        for (uint8_t i = 0; i < 4; i++) {
            pinMode(WHEELS[i].pinA, OUTPUT);
            pinMode(WHEELS[i].pinB, OUTPUT);
            digitalWrite(WHEELS[i].pinA, LOW);
            digitalWrite(WHEELS[i].pinB, LOW);
        }
    }

    // dir: +1 = Avanti (HIGH/LOW), -1 = Indietro (LOW/HIGH), 0 = Stop (LOW/LOW)
    inline void setWheelDir(uint8_t idx, int8_t dir) {
        if (idx >= 4) return;
        if (dir > 0) {
            digitalWrite(WHEELS[idx].pinA, HIGH);
            digitalWrite(WHEELS[idx].pinB, LOW);
        } else if (dir < 0) {
            digitalWrite(WHEELS[idx].pinA, LOW);
            digitalWrite(WHEELS[idx].pinB, HIGH);
        } else {
            digitalWrite(WHEELS[idx].pinA, LOW);
            digitalWrite(WHEELS[idx].pinB, LOW);
        }
    }

    inline void stopAll() {
        for (uint8_t i = 0; i < 4; i++) {
            setWheelDir(i, 0);
        }
    }

    inline void run() {
        // Forza nuovamente i pin come OUTPUT dopo runHardwareDiagnostics()
        init();

        Serial.println("\n==========================================");
        Serial.println(" [TEST MOTORI] Verifica digitale HIGH/LOW");
        Serial.println("==========================================");

        // Fase A: Test singola ruota a piena potenza (HIGH/LOW)
        for (uint8_t i = 0; i < 4; i++) {
            Serial.print(" -> Test in corso: ");
            Serial.println(WHEELS[i].label);

            Serial.println("    [AVANTI - IN1=HIGH, IN2=LOW]");
            setWheelDir(i, 1);
            delay(1000);

            setWheelDir(i, 0);
            delay(400);

            Serial.println("    [INDIETRO - IN1=LOW, IN2=HIGH]");
            setWheelDir(i, -1);
            delay(1000);

            setWheelDir(i, 0);
            delay(500);
        }

        // Fase B: Movimento simultaneo a spostamento netto nullo (tm = 300 ms)
        Serial.println(" -> Test Dondolio Simultaneo (Avanti 300ms / Indietro 300ms)");
        for (uint8_t i = 0; i < 4; i++) setWheelDir(i, 1);
        delay(300);
        stopAll();
        delay(200);
        for (uint8_t i = 0; i < 4; i++) setWheelDir(i, -1);
        delay(300);
        stopAll();

        Serial.println(" [TEST MOTORI] Completato!\n");
    }

} // namespace MotorTest

#endif // MOTOR_TEST_H