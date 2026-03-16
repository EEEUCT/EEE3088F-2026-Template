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
    // TODO: 1. Check if 'reg_addr' equals 'REG_SYS_STATUS'
    // TODO: 2. Check if 'data' equals 'STATUS_TRIG'
    // TODO: 3. If both are true, set 'my_registers.status' to 'STATUS_BUSY'
    //          This acknowledges the Master and starts the measurement phase.
}

/**
 * @brief Initializes the constant registers.
 * Called once at startup to satisfy ATP-SW-01 and ATP-SW-02.
 */
void init_registers(float contracted_ft, const char* my_id) {
    // HINT: Use memset() to zero out the whole struct first.
    // TODO: 1. Assign 'contracted_ft' to 'my_registers.discovery_freq'
    // TODO: 2. Use strncpy() to copy 'my_id' into 'my_registers.student_id'
    // TODO: 3. Set 'my_registers.status' to 'STATUS_READY'
}

/**
 * @brief The Main Processing Loop (M3 DSP Logic)
 * * Requirement: Total execution time between STATUS_TRIG and STATUS_READY 
 * must be <= 50ms.
 */
void process_doa_update(float d_mm, uint32_t fs) {
    // TODO: If 'my_registers.status' equals 'STATUS_BUSY':
    //       1. Call calculate_doa_angle_2mic(mic_L, mic_R, 1024, d_mm, fs)
    //       2. Store the returned angle in 'my_registers.doa_result'
    //       3. Set 'my_registers.confidence' to 100
    //       4. Set 'my_registers.status' back to 'STATUS_READY'
}
