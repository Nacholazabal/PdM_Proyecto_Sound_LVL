#include "port_rfid.h"
#include "stm32f4xx_hal.h"

// SPI handle (SPI3)
extern SPI_HandleTypeDef hspi3;

// GPIO for RC522 CS
#define RFID_CS_PORT   GPIOA
#define RFID_CS_PIN    GPIO_PIN_4

// RC522 command and register definitions
#define CMD_IDLE            0x00
#define CMD_TRANSCEIVE      0x0C

#define REG_COMMAND         0x01
#define REG_COM_I_EN        0x02
#define REG_DIV_I_EN        0x03
#define REG_COM_I_FLAG      0x04
#define REG_DIV_I_FLAG      0x05
#define REG_ERROR           0x06
#define REG_FIFO_DATA       0x09
#define REG_FIFO_LEVEL      0x0A
#define REG_CONTROL         0x0C
#define REG_BIT_FRAMING     0x0D
#define REG_MODE            0x11
#define REG_TX_CONTROL      0x14

// PICC commands
#define PICC_ANTICOLL       0x93
#define PICC_ANTICOLL_NVB   0x20

// Helpers to select/deselect
static inline void _cs_low(void)  { HAL_GPIO_WritePin(RFID_CS_PORT, RFID_CS_PIN, GPIO_PIN_RESET); }
static inline void _cs_high(void) { HAL_GPIO_WritePin(RFID_CS_PORT, RFID_CS_PIN, GPIO_PIN_SET); }

/**
 * @brief Write to a RC522 register over SPI.
 */
bool port_rfid_write_reg(uint8_t reg, uint8_t val) {
    uint8_t addr = (reg << 1) & 0x7E;
    _cs_low();
    HAL_SPI_Transmit(&hspi3, &addr, 1, 100);
    HAL_SPI_Transmit(&hspi3, &val,  1, 100);
    _cs_high();
    return true;
}

/**
 * @brief Read from a RC522 register over SPI.
 */
uint8_t port_rfid_read_reg(uint8_t reg) {
    uint8_t addr = ((reg << 1) & 0x7E) | 0x80;
    uint8_t val  = 0;
    _cs_low();
    HAL_SPI_Transmit(&hspi3, &addr, 1, 100);
    HAL_SPI_Receive(&hspi3, &val,  1, 100);
    _cs_high();
    return val;
}

/**
 * @brief Send a command + data through RC522 FIFO and await a response.
 */
bool port_rfid_command(const uint8_t *cmd, size_t cmd_len, uint8_t *response, size_t *resp_len) {
    // Idle & clear IRQ flags
    port_rfid_write_reg(REG_COMMAND, CMD_IDLE);
    port_rfid_write_reg(REG_COM_I_FLAG, 0x7F);
    // Flush FIFO
    port_rfid_write_reg(REG_FIFO_LEVEL, 0x80);

    // Write data into FIFO
    for (size_t i = 0; i < cmd_len; i++) {
        port_rfid_write_reg(REG_FIFO_DATA, cmd[i]);
    }
    // Execute Transceive
    port_rfid_write_reg(REG_COMMAND, CMD_TRANSCEIVE);
    // Start sending
    port_rfid_write_reg(REG_BIT_FRAMING, 0x80);

    // Wait for completion (ComIrq bit 0x30)
    uint8_t irq;
    uint32_t deadline = HAL_GetTick() + 100;
    do {
        irq = port_rfid_read_reg(REG_COM_I_FLAG);
    } while (!(irq & 0x30) && HAL_GetTick() < deadline);

    // Stop sending
    port_rfid_write_reg(REG_BIT_FRAMING, 0x00);

    // Check for errors
    uint8_t error = port_rfid_read_reg(REG_ERROR);
    if (error & 0x1B) {
        return false;
    }

    // Read how many bytes in FIFO
    uint8_t count = port_rfid_read_reg(REG_FIFO_LEVEL);
    if (count == 0 || count > *resp_len) {
        return false;
    }

    // Read out FIFO
    for (uint8_t i = 0; i < count; i++) {
        response[i] = port_rfid_read_reg(REG_FIFO_DATA);
    }
    *resp_len = count;
    return true;
}

/**
 * @brief Initialize the RC522 chip.
 */
bool port_rfid_init(void) {
    // Ensure CS is high
    _cs_high();
    HAL_Delay(50);

    // Reset via command
    port_rfid_write_reg(REG_COMMAND, CMD_IDLE);
    // Turn on antenna: TxControlReg bit 0x03
    port_rfid_write_reg(REG_TX_CONTROL, 0x03);
    return true;
}

/**
 * @brief Read one UID via anti‐collision cascade.
 */
bool port_rfid_read_uid(uint8_t *uid, size_t *uid_len) {
    uint8_t cmd[] = { PICC_ANTICOLL, PICC_ANTICOLL_NVB };
    uint8_t resp[10];
    size_t  resp_sz = sizeof(resp);

    if (!port_rfid_command(cmd, sizeof(cmd), resp, &resp_sz)) {
        return false;
    }
    // UID is first 5 bytes (4 UID + 1 BCC)
    if (resp_sz < 5) {
        return false;
    }
    memcpy(uid, resp, 4);
    *uid_len = 4;
    return true;
}
