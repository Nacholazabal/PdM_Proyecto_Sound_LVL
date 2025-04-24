/**
 * @file app_isr.h
 * @brief Rutinas de servicio de interrupción a nivel de aplicación.
 *
 * Este archivo declara funciones y variables globales para manejar interrupciones
 * del ADC con DMA y del botón vía GPIO.
 */

#ifndef APP_ISR_H
#define APP_ISR_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define ADC_BUFFER_SIZE 256 ///< Tamaño del buffer para lecturas por DMA

// Buffer DMA y nivel de envolvente calculado
extern uint16_t          adc_dma_buffer[ADC_BUFFER_SIZE];
extern volatile uint16_t envelope;

/**
 * Maneja la interrupción del botón.
 *
 * Se llama desde HAL_GPIO_EXTI_Callback() cuando la interrupción fue disparada por el pin del botón.
 */
void app_isr_button_handler(void);

#endif // APP_ISR_H
