/**
 * @file port_button.c
 * @brief Implementation of the hardware-specific functions for button input.
 *
 * This file contains the functions that directly interact with the STM32 HAL to read the
 * state of the button.
 */

#include "port_button.h"

// Board configuration for the button.
#define BUTTON_PULL_LOW     true        ///< true if the button is active-low

/**
 * @brief Reads the current state of the button.
 *
 * Uses HAL_GPIO_ReadPin to retrieve the button state. If the button is active-low,
 * the logic is inverted so that the function returns true when the button is pressed.
 *
 * @return true if the button is pressed, false if released.
 */
bool_t port_button_read(void) {
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
    return BUTTON_PULL_LOW ? (pin_state == GPIO_PIN_RESET) : (pin_state == GPIO_PIN_SET);
}
