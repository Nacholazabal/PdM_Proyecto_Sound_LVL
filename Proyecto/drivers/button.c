/**
 * @file button.c
 * @brief Implementation of the button driver with debouncing.
 *
 * This file implements a debouncing state machine that uses the hardware-specific
 * function port_button_read() to sample the button state. When a valid press-release
 * cycle is detected, an internal flag is set. The main application polls this flag via
 * button_was_pressed().
 */

#include "button.h"
#include "port_button.h"
#include "API_delay.h"  // Ensure this file provides delayInit, delayRead, etc.

#define DEBOUNCE_PERIOD_MS 40U

/**
 * @brief Enum for the debouncing states.
 */
typedef enum {
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RAISING
} button_state_t;

static button_state_t current_state;
static delay_t delay_debounce;
static bool_t button_flag;  ///< Flag set when a complete press-release is detected

/**
 * @brief Initializes the button state machine.
 */
void button_init(void) {
    current_state = BUTTON_UP;
    delayInit(&delay_debounce, DEBOUNCE_PERIOD_MS);
    button_flag = false;
}

/**
 * @brief Internal function called when a valid button release is confirmed.
 */
static void on_button_released(void) {
    button_flag = true;
}

/**
 * @brief Updates the debouncing state machine.
 *
 * This function reads the button status using port_button_read() and updates the
 * state machine. It should be called in the main loop periodically.
 */
void button_update(void) {
    bool_t button_status = port_button_read();  // true = pressed; false = released
    switch (current_state) {
        case BUTTON_UP:
            if (button_status == true) {
                current_state = BUTTON_FALLING;
                delayInit(&delay_debounce, DEBOUNCE_PERIOD_MS);
            }
            break;
        case BUTTON_FALLING:
            if (delayRead(&delay_debounce)) {
                if (button_status == true) {
                    current_state = BUTTON_DOWN;
                } else {
                    current_state = BUTTON_UP;
                }
            }
            break;
        case BUTTON_DOWN:
            if (button_status == false) {
                current_state = BUTTON_RAISING;
                delayInit(&delay_debounce, DEBOUNCE_PERIOD_MS);
            }
            break;
        case BUTTON_RAISING:
            if (delayRead(&delay_debounce)) {
                if (button_status == false) {
                    on_button_released();
                    current_state = BUTTON_UP;
                } else {
                    current_state = BUTTON_DOWN;
                }
            }
            break;
        default:
            button_init();
            break;
    }
}

/**
 * @brief Checks if a button press event has occurred.
 *
 * Returns true once per complete cycle (press and release) and resets the flag.
 *
 * @return true if an event is detected; false otherwise.
 */
bool_t button_was_pressed(void) {
    if (button_flag) {
        button_flag = false;
        return true;
    }
    return false;
}



