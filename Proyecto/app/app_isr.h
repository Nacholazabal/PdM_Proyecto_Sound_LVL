/**
 * @file app_isr.h
 * @brief Application-level interrupt service routines.
 *
 * This file declares functions for handling external interrupts at the application level.
 * In particular, it provides a function to handle the button interrupt event.
 * The HAL_GPIO_EXTI_Callback() override (placed in app_isr.c) calls these functions.
 */

#ifndef APP_ISR_H
#define APP_ISR_H
#include <stdint.h>
#include <stdbool.h>

#define ADC_BUFFER_SIZE 256

extern uint16_t          adc_dma_buffer[ADC_BUFFER_SIZE];
extern volatile uint16_t envelope;

/**
 * @brief Handles the button interrupt event.
 *
 * This function is called from HAL_GPIO_EXTI_Callback() when the button's GPIO pin triggers
 * an interrupt. It delegates the update to the button driver.
 */
void app_isr_button_handler(void);

#endif // APP_ISR_H
