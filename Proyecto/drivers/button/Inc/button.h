/**
 * @file button.h
 * @brief Public header for the button driver.
 *
 * This driver implements a debouncing state machine for a push button.
 * The main application can poll the flag provided by this driver to
 * detect a complete press-release cycle.
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>

typedef bool bool_t;
/**
 * @brief Initializes the button driver and its debouncing state machine.
 */
void button_init(void);

/**
 * @brief Updates the button state machine.
 *
 * This function should be called periodically (e.g., in the main loop)
 * to update the debouncing logic.
 */
void button_update(void);

/**
 * @brief Checks if a full button press (press-release cycle) has been detected.
 *
 * This function returns true only once per complete press-release event.
 *
 * @return true if a valid button press cycle was detected, false otherwise.
 */
bool_t button_was_pressed(void);

bool_t  button_was_long_pressed(void);

#endif // BUTTON_H
