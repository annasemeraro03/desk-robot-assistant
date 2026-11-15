#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "../../app/RobotState.h"

/**
 * @brief Inizializza il display OLED SH1106 1.3" su bus I2C condiviso.
 */
bool oledInit();

/**
 * @brief Pulisce il frame buffer locale in RAM (1024 byte).
 *        Non effettua traffico sul bus I2C.
 */
void oledClear();

/**
 * @brief Aggiorna l'intero display in un'unica transazione (solo per setup iniziale).
 */
void oledUpdate();

/**
 * @brief Trasmette sul bus I2C UNA SINGOLA PAGINA hardware da 128 byte (Specifica Sez. 4.2).
 *        Permette il Page-Chunking (8 pagine da ~2.3 ms ciascuna) per limitare il
 *        bloccaggio massimo a B_1 = 2.5 ms ed eliminare la Priority Inversion.
 *
 * @param page Indice della pagina hardware (0 .. 7).
 */
void oledUpdatePage(uint8_t page);

/**
 * @brief Scrive una stringa nel frame buffer in RAM alle coordinate indicate.
 */
void oledDrawText(uint8_t x, uint8_t y, const char* text);

/**
 * @brief Compone nel frame buffer in RAM la schermata testuale corrispondente
 *        allo stato corrente della FSM (senza occupare il bus I2C).
 *
 * @param snapshot Copia consistente dello stato corrente del robot.
 */
void oledRenderStateScreen(const RobotStateData *snapshot);

/**
 * @brief Mantenuta per compatibilita' con il codice precedente.
 */
void oledDrawFace(uint8_t mood);

#endif