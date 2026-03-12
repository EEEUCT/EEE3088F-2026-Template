#include "i2c_registers.h"
#include "doa_engine.h"
#include <string.h>

/* Global instance of the register map - used by I2C ISR and SiL Test Bench */
i2c_register_map_t my_registers;

/* Mock ADC Buffers (used for simulation and internal processing) */
int16_t mic_L[1024]; 
int16_t mic_R[1024];

/**
 * @brief Handles I2C Write events from the Master Rig.
 * This is where the Trigger (Reg 0x00) is detected.
 * * Requirement ATP-SW-03: This function must be extremely fast to 
 * maintain I2C stability. Do NOT run DSP code inside this function.
 */
void handle_i2c_write(uint8_t reg_addr, uint8_t data) {
    // TODO: Check if the Master is writing to the System Status register (REG_SYS_STATUS)
    // TODO: If the data written is the Trigger command (STATUS_TRIG),
    //       update the system status register to STATUS_BUSY.
    //       This is the handshake that starts the measurement process.
}

/**
 * @brief Initializes the constant registers.
 * Called once at startup to satisfy ATP-SW-01 and ATP-SW-02.
 */
void init_registers(float contracted_ft, const char* my_id) {
    // TODO: Initialize the my_registers struct.
    //       - Set the discovery frequency register from the 'contracted_ft' parameter.
    //       - Copy the student ID string into the correct register.
    //       - Set the initial system status to STATUS_READY.
    // HINT: Use memset() to zero out the whole struct first.
}

/**
 * @brief The Main Processing Loop (M3 DSP Logic)
 * * Requirement: Total execution time between STATUS_TRIG and STATUS_READY 
 * must be <= 50ms.
 */
void process_doa_update(float d_mm, uint32_t fs) {
    // TODO: Check if the system status is STATUS_BUSY.
    // TODO: If it is, call your 'calculate_doa_angle_2mic' function.
    // TODO: Store the angle result in the 'doa_result' register.
    // TODO: Set a static confidence value (e.g., 100).
    // TODO: Finally, update the system status register to STATUS_READY to signal
    //       that the measurement is complete and data is valid.
}
