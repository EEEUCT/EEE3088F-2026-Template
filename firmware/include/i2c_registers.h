#ifndef I2C_REGISTERS_H
#define I2C_REGISTERS_H

#include <stdint.h>

/**
 * MANDATORY I2C CONFIGURATION
 * Address: 0x42
 */
#define SLAVE_ADDRESS 0x42

/**
 * REGISTER MAP DEFINITIONS
 */
#define REG_SYS_STATUS      0x00    // R/W: 0x01 (Trig), 0x02 (Busy), 0x03 (Ready)
#define REG_DISCOVERY_FREQ  0x04    // R: 32-bit float (Contracted ft)
#define REG_DOA_RESULT      0x08    // R: int8 (-90 to +90 degrees)
#define REG_CONFIDENCE      0x0C    // R: uint8 (0-100)
#define REG_STUDENT_ID      0x10    // R: char[9] (e.g., "ABCXYZ001")

/**
 * STATUS CODES (for REG_SYS_STATUS)
 */
typedef enum {
    STATUS_TRIG  = 0x01,
    STATUS_BUSY  = 0x02,
    STATUS_READY = 0x03
} system_status_t;

/**
 * I2C REGISTER STRUCTURE (Recommended Implementation)
 * This structure aligns with the 50ms timing budget by allowing 
 * memory-mapped I2C access.
 */
typedef struct __attribute__((packed)) {
    volatile uint8_t  status;          // Offset 0x00
    uint8_t           reserved1[3];    // Padding to 0x04
    float             discovery_freq;  // Offset 0x04
    int8_t            doa_result;      // Offset 0x08
    uint8_t           reserved2[3];    // Padding to 0x0C
    uint8_t           confidence;      // Offset 0x0C
    uint8_t           reserved3[3];    // Padding to 0x10
    char              student_id[9];   // Offset 0x10
} i2c_register_map_t;

#endif
