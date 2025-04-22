#include "button.h"
#include "port_button.h"
#include <string.h>
#include "API_delay.h"
#define DEBOUNCE_MS    40U
#define HOLD_MS      5000U

typedef enum { UP, FALLING, DOWN, RISING } State;

static State       state;
static bool_t      shortFlag;
static bool_t      longFlag;
static delay_t     dbDelay;
static delay_t     holdDelay;

void button_init(void) {
    state      = UP;
    shortFlag  = false;
    longFlag   = false;
    delayInit(&dbDelay, DEBOUNCE_MS);
}

void button_update(void) {
    bool_t phys = port_button_read();

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
                    // start hold timer
                    delayInit(&holdDelay, HOLD_MS);
                    longFlag = false;
                } else {
                    state = UP;
                }
            }
            break;

        case DOWN:
            // check for hold
            if (delayRead(&holdDelay) && !longFlag) {
                longFlag = true;
            }
            if (!phys) {
                state = RISING;
                delayInit(&dbDelay, DEBOUNCE_MS);
            }
            break;

        case RISING:
            if (delayRead(&dbDelay)) {
                if (!phys) {
                    // on stable release:
                    if (longFlag) {
                        // generate long‑press event
                        // shortFlag stays false
                    } else {
                        shortFlag = true;
                    }
                    state = UP;
                } else {
                    state = DOWN;
                }
            }
            break;
    }
}

bool_t button_was_pressed(void) {
    if (shortFlag) {
        shortFlag = false;
        return true;
    }
    return false;
}

bool_t button_was_long_pressed(void) {
    if (longFlag) {
        longFlag = false;
        return true;
    }
    return false;
}
