#ifndef PORT_RFID_H
#define PORT_RFID_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/**
 * @brief Low‐level init of the RC522 over SPI3.
 */
bool port_rfid_init(void);

/**
 * @brief Low‐level read of a card’s UID.
 * @param uid      Buffer for UID bytes.
 * @param uid_len  On input, buffer size; on output, bytes actually read.
 * @return true if a UID was read.
 */
bool port_rfid_read_uid(uint8_t *uid, size_t *uid_len);

/**
 * @brief Write a single RC522 register.
 */
bool port_rfid_write_reg(uint8_t reg, uint8_t val);

/**
 * @brief Read a single RC522 register.
 */
uint8_t port_rfid_read_reg(uint8_t reg);

/**
 * @brief Send raw command to RC522 FIFO and receive raw response.
 * @param cmd         Command bytes.
 * @param cmd_len     Number of bytes in cmd[].
 * @param response    Buffer to receive returned bytes.
 * @param resp_len    On output, number of bytes returned.
 * @return true on success.
 */
bool port_rfid_command(const uint8_t *cmd, size_t cmd_len, uint8_t *response, size_t *resp_len);

#endif // PORT_RFID_H
