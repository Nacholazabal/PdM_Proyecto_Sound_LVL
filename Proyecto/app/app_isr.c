/**
 * @file app_isr.c
 * @brief Implementación de interrupciones a nivel de aplicación.
 *
 * Este módulo define el callback de interrupción GPIO y la lógica para el cálculo de nivel de sonido.
 */

#include "app_isr.h"
#include "button.h"
#include "port_button.h"
#include "app.h"
#include "debug_uart.h"

// Buffer DMA donde se almacenan muestras del ADC
uint16_t adc_dma_buffer[ADC_BUFFER_SIZE];
// Nivel de sonido (envolvente), usado por el sistema principal
volatile uint16_t envelope = 0;

//----------------------------------------------------------------------------------------------------------------------
/**
 * Callback global de interrupción externa (GPIO).
 *
 * Es llamado por la HAL cuando ocurre una interrupción. Si proviene del botón,
 * llama al handler correspondiente.
 *
 * @param GPIO_Pin Número de pin que generó la interrupción.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BUTTON_PIN)
    {
        app_isr_button_handler();
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Handler del botón a nivel de aplicación.
 *
 * Llama al manejador del botón para registrar el evento.
 */
void app_isr_button_handler(void)
{
    // Actualiza la máquina de estados del botón (antirrebote)
    button_update();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Callback de llenado de buffer del ADC con DMA.
 *
 * Calcula el nivel RMS del buffer del ADC y lo guarda en envelope.
 *
 * @param hadc Puntero al handler del ADC.
 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // Verifico que la interrupción venga del ADC1
    if (hadc->Instance != ADC1){
        return;
    }

    // 1) Calcular el nivel de offset (DC bias) promedio
    uint32_t sum = 0;
    for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
        sum += adc_dma_buffer[i];
    }
    float bias = (float)sum / (float)ADC_BUFFER_SIZE;

    // 2) Calcular la suma de los cuadrados de la señal centrada en el bias
    uint32_t sum_sq = 0;
    for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
        float d = (float)adc_dma_buffer[i] - bias;
        sum_sq += (uint32_t)(d * d);
    }

    // 3) Obtener RMS: raíz cuadrada del promedio de los cuadrados
    float rms = sqrtf((float)sum_sq / (float)ADC_BUFFER_SIZE);

    // 4) Guardar el valor RMS en la variable global envelope
    envelope = rms;
}


