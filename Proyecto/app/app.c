/**********************************************************************************************************************
 * Copyright (c) 2024, Your Name
 *
 * This file is part of the Voice Recorder project.
 *
 * @file app.c
 * @brief Main application code for the voice recorder.
 **********************************************************************************************************************/

#include "stm32f4xx_hal.h"
#include "main.h"
#include "app.h"
#include "button.h"
#include "usb_commands.h"
#include "usb_cdc.h"
#include "string.h"
#include "debug_uart.h"


/**
 * @brief Enumeration of the main application states.
 */
typedef enum {
    INITIALIZING = 0,  ///< System is initializing.
    IDLE,              ///< Waiting for command (button press or USB command).
    RECORDING,         ///< Recording audio using ADC + DMA.
    PROCESSING,        ///< Processing/saving recorded audio to SD card.
    USB_COMMAND,       ///< Handling USB CDC commands.
    APP_ERROR              ///< Error state.
} MainApplicationState;

MainApplicationState application_state;  ///< Global application state variable

// Forward declarations of state handler functions.
static void on_initializing(void);
static void on_idle(void);
static void on_recording(void);
static void on_processing(void);
static void on_usb_command(void);
static void on_app_error(void);

/**
 * @brief Entry point for the application.
 *
 * This function implements the main loop of the state machine.
 */
void app_entry_point(void)
{
    application_state = INITIALIZING;

    while (1) {
        switch (application_state) {
            case INITIALIZING:
                on_initializing();
                break;
            case IDLE:
                on_idle();
                break;
            case RECORDING:
                on_recording();
                break;
            case PROCESSING:
                on_processing();
                break;
            case USB_COMMAND:
                on_usb_command();
                break;
            case APP_ERROR:
            default:
                on_app_error();
                break;
        }
    }
}

/**
 * @brief Handles the INITIALIZING state.
 *
 * Initializes all necessary modules and peripherals (e.g., button, mic, OLED, SD, USB).
 * After the initialization completes, transitions to IDLE state.
 */
static void on_initializing(void)
{
    // Initialize the button driver (debouncing state machine and HAL interface)
    button_init();

    // Initialize the display
    //oled_init();        // This calls port_oled_init() and clears the display
    //oled_clear();
    //oled_set_cursor(0, 0);
    //oled_print("App initialized");

    usb_cdc_init();

    debug_uart_init();
    debug_uart_print("App initialized\r\n");
    // TODO: Initialize other drivers (mic, oled, SD, USB, etc.)

    // Once all peripheral initialization is completed, transition to IDLE.
    application_state = IDLE;
}

/**
 * @brief Handles the IDLE state.
 *
 * In the IDLE state, the system polls the button and listens for USB commands.
 * When a complete button press (press-release) is detected, the state transitions
 * to RECORDING.
 */
static void on_idle(void)
{
    // Update the button's debounce logic
    button_update();

    // Process any button event as needed (e.g., button press triggers recording)
    if (button_was_pressed()) {
        // For demonstration, toggle LED to indicate button event.
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        // Optionally, transition to another state:
        // application_state = RECORDING;
    }

    // Poll USB CDC for a complete command
    if (usb_cdc_isCommandPending()) {
    	debug_uart_print("COMMAND PENDING\r\n");
        // Transition to the USB_COMMAND state for processing the command.
        application_state = USB_COMMAND;
    }
}

/**
 * @brief Handles the RECORDING state.
 *
 * In this state, audio is captured using ADC with DMA and stored in RAM.
 * The USB interface is disabled during the recording.
 *
 * After the recording is stopped (either by fixed duration or button press),
 * the state transitions to PROCESSING.
 */
static void on_recording(void)
{
    // TODO: Start ADC conversion with DMA.
    // Record audio into a RAM buffer.

    // For demonstration, simulate recording done by transitioning immediately.
    application_state = PROCESSING;
}

/**
 * @brief Handles the PROCESSING state.
 *
 * In this state, the recorded audio data (from RAM) is saved to the SD card using SPI + FatFs.
 * Once the data is saved, the system transitions back to the IDLE state.
 */
static void on_processing(void)
{
    // TODO: Implement saving of audio data to the SD card.

    // After processing, return to the IDLE state.
    application_state = IDLE;
}

/**
 * @brief Handles the USB_COMMAND state.
 *
 * Processes USB CDC commands (e.g., list files, send file, delete file).
 * After executing a command, the state transitions back to the IDLE state.
 */
static void on_usb_command(void)
{

    const char* cmd = usb_cdc_getCommand();
    debug_uart_print("COMMAND PROCESSING\r\n");

    if (strcmp(cmd, USB_CMD_LED_ON) == 0)
    {
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        usb_cdc_sendString("LED turned on\r\n");
    }
    else if (strcmp(cmd, USB_CMD_LED_OFF) == 0)
    {
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
        usb_cdc_sendString("LED turned off\r\n");
    }
    else
    {
        usb_cdc_sendString("Unknown command\r\n");
    }

    // Clear the command so that the flag is reset.
    usb_cdc_clearCommand();

    // Once command processing is completed, transition back to IDLE.
    application_state = IDLE;
}

/**
 * @brief Handles the ERROR state.
 *
 * In case of errors (e.g., SD initialization failure, DMA error), the system enters
 * the ERROR state and can provide visual or debugging cues.
 */
static void on_app_error(void)
{
    // TODO: Implement error handling (e.g., blink an LED to indicate error).
    while (1) {
        // Stay here to indicate an error condition.
    }
}
