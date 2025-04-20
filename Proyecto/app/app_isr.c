/**
 * @file app_isr.c
 * @brief Implementation of application-level interrupt service routines.
 *
 * This module provides the ISR handler for the button by overriding the HAL_GPIO_EXTI_Callback().
 * It ensures that hardware-level interrupts are translated into high-level events used by the drivers.
 */

#include "app_isr.h"
#include "button.h"
#include "port_button.h"  // This should define BUTTON_PIN
#include "app.h"
#include "debug_uart.h"

uint16_t adc_dma_buffer[ADC_BUFFER_SIZE];
volatile uint16_t envelope = 0;
/**
 * @brief Override of the HAL GPIO EXTI Callback.
 *
 * This function is called by the HAL when an external interrupt occurs.
 * It checks if the interrupt source is the button pin and, if so, calls the
 * application-level button handler.
 *
 * @param GPIO_Pin The pin number that triggered the interrupt.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // Check if the interrupt came from the defined button pin.
    if (GPIO_Pin == BUTTON_PIN)
    {
        // Call the application ISR handler for the button.
        app_isr_button_handler();
    }
    // For other pins, add additional handlers if needed.
}

/**
 * @brief Application-level button ISR handler.
 *
 * This function updates the button driver by calling its update function.
 * It is invoked by the HAL_GPIO_EXTI_Callback() override when the button's
 * external interrupt is triggered.
 */
void app_isr_button_handler(void)
{
    // Update the debouncing state machine in the button driver.
    button_update();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        // 1) Compute DC bias
        uint32_t sum = 0;
        for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
            sum += adc_dma_buffer[i];
        }
        uint16_t bias = sum / ADC_BUFFER_SIZE;

        // 2) Compute mean absolute deviation (“envelope”)
        uint32_t abs_sum = 0;
        for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
            int32_t d = (int32_t)adc_dma_buffer[i] - (int32_t)bias;
            abs_sum += (d < 0 ? -d : d);
        }
        envelope = abs_sum / ADC_BUFFER_SIZE;
    }
}


