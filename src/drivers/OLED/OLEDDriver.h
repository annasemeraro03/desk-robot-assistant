#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <stdint.h>

bool oledInit();

void oledClear();

void oledUpdate();

void oledDrawText(uint8_t x, uint8_t y, const char* text);

void oledDrawFace(uint8_t mood);

#endif