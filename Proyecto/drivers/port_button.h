/**
 * @file port_button.h
 * @brief Hardware abstraction layer for button reading.
 *
 * This file declares the function to read the physical state of the button.
 */

#ifndef PORT_BUTTON_H
#define PORT_BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef bool bool_t;

#define BUTTON_PORT         GPIOC       ///< Button port (e.g., blue user button)
#define BUTTON_PIN          GPIO_PIN_13 ///< Button pin

/**
 * @brief Reads the physical state of the button.
 *
 * This function accesses the GPIO pin associated with the button and returns its state.
 *
 * @return true if the button is pressed, false if released.
 */
bool_t port_button_read(void);

#endif // PORT_BUTTON_H
