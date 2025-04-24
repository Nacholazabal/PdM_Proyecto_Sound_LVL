/**********************************************************************************************************************
Copyright (c) 2025, Ignacio Olazabal nacholazabal@gmail.com
**********************************************************************************************************************/

#pragma once

/**
 * @file app.h
 * @brief Main application header for the sound level detector project.
 *
 * This module defines the public API and global declarations for the core application state machine.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the main application state machine.
 *
 * Sets up all drivers and loads configuration from EEPROM, prepares peripherals
 * and transitions the system to IDLE state.
 */
void appInit(void);

/**
 * @brief Updates the application state machine.
 *
 * This function should be called periodically. It handles the state transitions
 * and main logic of the sound level detection system.
 */
void appUpdate(void);

#ifdef __cplusplus
}
#endif
