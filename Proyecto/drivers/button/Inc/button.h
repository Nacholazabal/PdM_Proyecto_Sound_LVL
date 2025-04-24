/**
 * @file button.h
 * @brief Módulo de manejo de botón con antirrebote
 *
 * Este driver implementa una máquina de estados para antirrebote
 * y permite detectar eventos de pulsación corta y larga.
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>

typedef bool bool_t;

/**
 * Inicializa el módulo del botón y la máquina de estados de antirrebote.
 */
void button_init(void);

/**
 * Actualiza el estado del botón.
 *
 * Debe llamarse periódicamente desde el bucle principal.
 */
void button_update(void);

/**
 * Devuelve true si se detectó un clic corto (presionar y soltar).
 */
bool_t button_was_pressed(void);

/**
 * Devuelve true si se detectó una pulsación larga.
 */
bool_t button_was_long_pressed(void);

#endif // BUTTON_H
