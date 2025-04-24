/**
 * @file port_button.h
 * @brief Capa de abstracción de hardware para la lectura del botón.
 *
 * Este archivo declara la función que permite leer el estado físico del botón.
 */

#ifndef PORT_BUTTON_H
#define PORT_BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef bool bool_t;

#define BUTTON_PORT         GPIOC       ///< Puerto del botón (ej. botón azul de usuario)
#define BUTTON_PIN          GPIO_PIN_13 ///< Pin del botón

/**
 * Lee el estado físico del botón.
 *
 * Esta función accede al pin GPIO correspondiente al botón y devuelve su estado lógico.
 *
 * @return true si el botón está presionado, false si está liberado.
 */
bool_t port_button_read(void);

#endif // PORT_BUTTON_H
