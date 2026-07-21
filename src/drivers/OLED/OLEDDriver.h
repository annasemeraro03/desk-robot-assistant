#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <stdint.h>

/**
 * @brief Initialize the OLED display.
 *
 * @return true if initialization succeeds.
 * @return false if initialization fails.
 */
bool oledInit();

/**
 * @brief Clear the OLED display buffer.
 */
void oledClear();

/**
 * @brief Update the OLED display.
 */
void oledUpdate();

/**
 * @brief Draw text on the OLED display.
 *
 * @param x Horizontal position.
 * @param y Vertical position.
 * @param text Text string to display.
 */
void oledDrawText(uint8_t x, uint8_t y, const char* text);

/**
 * @brief Draw robot face according to mood.
 *
 * @param mood Robot mood identifier.
 */
void oledDrawFace(uint8_t mood);

#endif