/**
 * @file usb_commands.h
 * @brief Módulo de interpretación de comandos USB CDC.
 *
 * Este archivo define los comandos disponibles que pueden ser enviados por USB,
 * y la estructura para almacenar acciones pendientes como configurar umbrales o la hora.
 */

#ifndef USB_COMMANDS_H
#define USB_COMMANDS_H

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"  // Para rtc_datetime_t
typedef bool bool_t;
// Texto de los comandos reconocidos
#define USB_CMD_GET_THRESH  "getth"
#define USB_CMD_SET_THRESH  "setth"
#define USB_CMD_GET_TIME    "gettime"
#define USB_CMD_SET_TIME    "settime"
#define USB_CMD_GET_LOG     "getlog"
#define USB_CMD_HELP        "help"

// Enumeración de comandos parseados
typedef enum {
    CMD_GET_THRESH,
    CMD_SET_THRESH,
    CMD_GET_TIME,
    CMD_SET_TIME,
    CMD_GET_LOG,
    CMD_HELP
} usb_command_t;

// Estructura para transportar parámetros de comandos SET_THRESH y SET_TIME
typedef struct {
    usb_command_t   cmd;
    uint16_t        low;
    uint16_t        high;
    rtc_datetime_t  dt;
} pending_action_t;

/**
 * @brief Inicializa el sistema de comandos USB.
 *
 * Debe llamarse una vez antes de procesar comandos.
 */
void usb_commands_init(void);

/**
 * @brief Verifica si hay un comando recibido pendiente de procesar.
 *
 * @return true si hay un comando disponible, false si no.
 */
bool_t usb_commands_pending(void);

/**
 * @brief Devuelve el comando recibido y lo interpreta.
 *
 * Parsea el texto del comando recibido y rellena la estructura con los parámetros asociados.
 *
 * @param[out] action Puntero a estructura donde se almacenará el comando y sus argumentos.
 * @return Enumeración del comando interpretado.
 */
usb_command_t usb_commands_get(pending_action_t *action);

/**
 * @brief Imprime el listado de comandos disponibles por USB.
 */
void usb_commands_print_help(void);

#endif // USB_COMMANDS_H
