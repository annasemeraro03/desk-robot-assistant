#include "OLEDDriver.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <stdio.h>

#include "../../config/Pins.h"
#include "../../config/Config.h"
#include "../../app/RobotConfig.h"

// Istanza driver SH1106 (buffer in RAM da 128x64 pixel = 1024 byte)
static Adafruit_SH1106G display(
    DISPLAY_WIDTH,
    DISPLAY_HEIGHT,
    &Wire,
    DISPLAY_RESET
);

bool oledInit() {
    if (!display.begin(DISPLAY_I2C_ADDRESS, true)) {
        Serial.println("[OLED] Inizializzazione SH1106 fallita");
        return false;
    }

    // Assicura che il bus I2C operi in Fast Mode (400 kHz)
    Wire.setClock(I2C_FAST_MODE_HZ);

    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Hello");
    display.display();
    return true;
}

void oledClear() {
    display.clearDisplay();
}

void oledUpdate() {
    display.display();
}

void oledUpdatePage(uint8_t page) {
    if (page >= DISPLAY_PAGES) {
        return;
    }

    uint8_t *buffer = display.getBuffer();
    if (buffer == nullptr) {
        return;
    }

    // 1. Invio comandi di posizionamento pagina (0xB0 | page) e colonna (offset +2 del chip SH1106)
    Wire.beginTransmission(DISPLAY_I2C_ADDRESS);
    Wire.write(0x00);                       // Control byte: flusso comandi
    Wire.write(0xB0 | (page & 0x07));       // Indirizzo pagina (0..7)
    Wire.write(0x02);                       // Lower column address = 2 (offset display 1.3" SH1106)
    Wire.write(0x10);                       // Higher column address = 0
    Wire.endTransmission();

    // 2. Invio dei 128 byte della pagina suddivisi in 2 blocchi da 64 byte
    //    Tempo totale di trasmissione a 400 kHz: ~2.3 ms <= C_cs,I2C = 2.5 ms
    const uint8_t *pagePtr = buffer + ((uint16_t)page * DISPLAY_PAGE_BYTES);
    for (uint8_t chunk = 0; chunk < 2; ++chunk) {
        Wire.beginTransmission(DISPLAY_I2C_ADDRESS);
        Wire.write(0x40);                   // Control byte: flusso dati RAM grafica
        Wire.write(pagePtr + (chunk * 64), 64);
        Wire.endTransmission();
    }
}

void oledDrawText(uint8_t x, uint8_t y, const char* text) {
    display.setCursor(x, y);
    display.setTextSize(1);
    display.println(text);
}

void oledRenderStateScreen(const RobotStateData *snapshot) {
    if (snapshot == nullptr) {
        return;
    }

    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);

    // Intestazione fissa in alto
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("=== ROBOT EMOTIVO ===");
    display.drawLine(0, 10, 127, 10, SH110X_WHITE);

    // Visualizzazione chiara dello stato corrente (obiettivo di questo step)
    display.setCursor(0, 16);
    display.print("STATO:");

    display.setCursor(0, 28);
    switch (snapshot->currentState) {
        case STATE_ACCESA:
            display.setTextSize(2);
            display.println("Hello");
            display.setTextSize(1);
            display.println("(ACCESA)");
            break;

        case STATE_FELICE:
            display.setTextSize(2);
            display.println("FELICE");
            display.setTextSize(1);
            {
                char buf[32];
                uint32_t secIdle = snapshot->inactivityElapsedMs / 1000UL;
                snprintf(buf, sizeof(buf), "Inattivita': %lus", (unsigned long)secIdle);
                display.println(buf);
            }
            break;

        case STATE_ANNOIATA:
            display.setTextSize(2);
            display.println("ANNOIATA");
            display.setTextSize(1);
            display.println("Fai una carezza!");
            break;

        case STATE_COCCOLATA:
            display.setTextSize(2);
            display.println("COCCOLATA");
            display.setTextSize(1);
            if (snapshot->focusSessionActive) {
                display.println("(Pausa Pomodoro)");
            } else {
                display.println("<3 <3 <3");
            }
            break;

        case STATE_CONCENTRATA:
            display.setTextSize(1);
            display.println(">> CONCENTRATA <<");
            {
                char buf[32];
                uint32_t remSec = (snapshot->focusRemainingMs + 999UL) / 1000UL;
                uint32_t mm = remSec / 60UL;
                uint32_t ss = remSec % 60UL;
                display.setCursor(0, 42);
                display.setTextSize(2);
                snprintf(buf, sizeof(buf), "%02lu:%02lu", (unsigned long)mm, (unsigned long)ss);
                display.println(buf);
            }
            break;

        case STATE_VITTORIA:
            display.setTextSize(2);
            display.println("VITTORIA!");
            display.setTextSize(1);
            display.println("Sessione completata");
            break;

        case STATE_INFORMAZIONI_CALENDARIO:
            display.setTextSize(1);
            display.println("INFO CALENDARIO");
            display.println(robotConfig.nextCalendarEvent);
            if (snapshot->focusSessionActive) {
                uint32_t remSec = (snapshot->focusRemainingMs + 999UL) / 1000UL;
                char buf[32];
                snprintf(buf, sizeof(buf), "Pomodoro congelato:%lus", (unsigned long)remSec);
                display.println(buf);
            } else {
                display.println("[1 Tap per uscire]");
            }
            break;

        case STATE_INFORMAZIONI_METEO:
            display.setTextSize(1);
            display.println("INFO METEO");
            display.print(robotConfig.city);
            display.print(": ");
            display.println(robotConfig.weatherSummary);
            display.println("[1 Tap per uscire]");
            break;

        case STATE_ARRABBIATA:
            display.setTextSize(2);
            display.println("ARRABBIATA");
            display.setTextSize(1);
            if (snapshot->focusSessionActive) {
                uint32_t remSec = (snapshot->focusRemainingMs + 999UL) / 1000UL;
                char buf[32];
                snprintf(buf, sizeof(buf), "Timer in pausa: %lus", (unsigned long)remSec);
                display.println(buf);
            } else {
                display.println("Non scuotermi!");
            }
            break;
    }
}

void oledDrawFace(uint8_t mood) {
    (void)mood;
}