#ifndef RFID_H
#define RFID_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define RFID_UID_MAX_LEN  5    // RC522 returns 5‐byte UID
#define RFID_MAX_CARDS    10   // Max cards we can register

/**
 * @brief Initialize the RFID reader (RC522).
 * @return true on success.
 */
bool rfid_init(void);

/**
 * @brief Read one card’s UID.
 * @param uid      Buffer to receive the UID bytes.
 * @param uid_len  On input, size of buffer; on output, actual UID length.
 * @return true if a card was read.
 */
bool rfid_read_uid(uint8_t *uid, size_t *uid_len);

/**
 * @brief Register a UID to the “allowed” list.
 * @param uid      UID bytes.
 * @param uid_len  Length of UID.
 * @return true on success.
 */
bool rfid_register_card(const uint8_t *uid, size_t uid_len);

/**
 * @brief Check whether a given UID is in the registered list.
 * @param uid      UID bytes.
 * @param uid_len  Length of UID.
 * @return true if it matches a registered card.
 */
bool rfid_authenticate(const uint8_t *uid, size_t uid_len);

#endif // RFID_H
