/**
 * @file button.c
 * @brief Implementación del driver de botón con detección de rebotes y pulsaciones.
 *
 * Este módulo utiliza una máquina de estados para manejar el botón físico,
 * detectando pulsaciones cortas y largas con timers para debounce y hold.
 */

#include <string.h>

#include "API_delay.h"
#include "button.h"
#include "port_button.h"

#define DEBOUNCE_MS    40U    ///< Tiempo de debounce en milisegundos
#define HOLD_MS       5000U   ///< Tiempo para considerar pulsación larga

typedef enum { UP, FALLING, DOWN, RISING } State;

static State       state;
static bool_t      shortFlag;
static bool_t      longFlag;
static delay_t     dbDelay;
static delay_t     holdDelay;

/**
 * Inicializa el estado inicial y los timers.
 */
void button_init(void) {
    state      = UP;
    shortFlag  = false;
    longFlag   = false;
    delayInit(&dbDelay, DEBOUNCE_MS);
}

/**
 * Actualiza la máquina de estados del botón.
 *
 * Se encarga del manejo de rebotes y detección de pulsaciones largas o cortas.
 */
void button_update(void) {
    bool_t phys = port_button_read(); // Lectura física del botón

    switch(state) {
        case UP:
            if (phys) {
                state = FALLING;
                delayInit(&dbDelay, DEBOUNCE_MS);
            }
            break;

        case FALLING:
            if (delayRead(&dbDelay)) {
                if (phys) {
                    state = DOWN;
                    delayInit(&holdDelay, HOLD_MS); // Comenzar conteo para pulsación larga
                    longFlag = false;
                } else {
                    state = UP; // Falsa alarma
                }
            }
            break;

        case DOWN:
            if (delayRead(&holdDelay) && !longFlag) {
                longFlag = true; // Se detecta pulsación larga
            }
            if (!phys) {
                state = RISING;
                delayInit(&dbDelay, DEBOUNCE_MS);
            }
            break;

        case RISING:
            if (delayRead(&dbDelay)) {
                if (!phys) {
                    if (longFlag) {
                        // No se genera shortFlag en caso de long
                    } else {
                        shortFlag = true; // Pulsación corta válida
                    }
                    state = UP;
                } else {
                    state = DOWN; // Rebote, seguir abajo
                }
            }
            break;
    }
}

/**
 * Verifica si hubo una pulsación corta desde la última consulta.
 */
bool_t button_was_pressed(void) {
    if (shortFlag) {
        shortFlag = false;
        return true;
    }
    return false;
}

/**
 * Verifica si hubo una pulsación larga desde la última consulta.
 */
bool_t button_was_long_pressed(void) {
    if (longFlag) {
        longFlag = false;
        return true;
    }
    return false;
}

