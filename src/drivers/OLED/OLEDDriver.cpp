#include "OLEDDriver.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "../../config/Pins.h"
#include "../../config/Config.h"

// ===============================
//      OLED Display Instance
// ===============================

// OLED display instance
static Adafruit_SH1106G display(
    DISPLAY_WIDTH,
    DISPLAY_HEIGHT,
    &Wire,
    DISPLAY_RESET
);

/**
 * @brief Initialize the OLED display.
 *
 * Initializes the I2C communication and configures the SH1106 OLED display.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool oledInit(){
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);               // Initialize I2C communication

    // Initialize OLED display
    if(!display.begin(DISPLAY_I2C_ADDRESS, true)){      
        Serial.println("OLED initialization failed");
        return false;
    }

    // Configure display parameters
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);

    // Display startup message
    display.println("Desk Robot");
    display.display();
    return true;
}

/**
 * @brief Clear the OLED display buffer.
 */
void oledClear(){
    display.clearDisplay();
}

/**
 * @brief Send the display buffer to the OLED.
 *
 * Updates the physical display with the current content stored in the buffer.
 */
void oledUpdate(){
    display.display();
}

/**
 * @brief Draw text on the OLED display.
 *
 * @param x Horizontal position.
 * @param y Vertical position.
 * @param text Text string to display.
 */
void oledDrawText(uint8_t x, uint8_t y, const char* text){
    display.setCursor(x, y);
    display.setTextSize(1);
    display.println(text);
}

/**
 * @brief Draw the robot face according to its mood.
 *
 * Displays a simple graphical representation
 * of the current robot emotional state.
 *
 * @param mood Current robot mood identifier.
 */
void oledDrawFace(uint8_t mood){
    display.clearDisplay();         // Clear previous frame

    // Draw robot eyes
    display.fillCircle(35, 30, 8, SH110X_WHITE);
    display.fillCircle(90, 30, 8, SH110X_WHITE);

    // Draw mouth according to mood
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