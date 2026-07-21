#include "OLEDDriver.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "../../config/Pins.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

static Adafruit_SH1106G display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET
);

bool oledInit(){
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if(!display.begin(0x3C, true)){
        Serial.println("OLED initialization failed");
        return false;
    }

    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("Desk Robot");
    display.display();
    return true;
}

void oledClear(){display.clearDisplay();}

void oledUpdate(){display.display();}

void oledDrawText(uint8_t x, uint8_t y, const char* text){
    display.setCursor(x, y);
    display.setTextSize(1);
    display.println(text);
}

void oledDrawFace(uint8_t mood){
    display.clearDisplay();
    // Eyes
    display.fillCircle(35, 30, 8, SH110X_WHITE);
    display.fillCircle(90, 30, 8, SH110X_WHITE);

    switch(mood){
        case 0: {
            // Happy
            display.drawCircle(64, 45, 15, SH110X_WHITE);
            break;
        }

        case 1: {
            // Bored
            display.drawLine(50, 45, 78, 45, SH110X_WHITE);
            break;
        }

        case 2: {
            // Sleepy
            display.drawLine(25, 30, 45, 30, SH110X_WHITE);
            break;
        }
    }
}