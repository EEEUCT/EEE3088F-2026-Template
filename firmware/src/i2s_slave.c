#include "i2c_registers.h"
#include "doa_engine.h"
#include <string.h>

/* Global instance of the register map */
i2c_register_map_t my_registers;

/**
 * @brief Handles I2C Write events from the Master Rig.
 * This is where the Trigger (Reg 0x00) is detected.
 */
void handle_i2c_write(uint8_t reg_addr, uint8_t data) {
    if (reg_addr == REG_SYS_STATUS && data == STATUS_TRIG) {
        /* ATP-SW-03: Transition to BUSY immediately */
        my_registers.status = STATUS_BUSY;

        /* * TODO: Trigger your DOA calculation here.
         * Note: In a real system, you should flag this for the main loop 
         * rather than running heavy DSP inside an interrupt.
         */
    }
}

/**
 * @brief Example of how to initialize the constant registers.
 */
void init_registers(float contracted_ft, const char* my_id) {
    memset(&my_registers, 0, sizeof(i2c_register_map_t));
    
    /* ATP-SW-01 & 02: Pre-load the Discovery constants */
    my_registers.discovery_freq = contracted_ft;
    strncpy(my_registers.student_id, my_id, 9);
    
    my_registers.status = STATUS_READY;
}

/**
 * @brief The Main Processing Loop (Conceptual)
 */
void process_doa_update() {
    if (my_registers.status == STATUS_BUSY) {
        /* TODO: Call calculate_doa_angle_2mic(...) */
        
        /* TODO: Update REG_DOA_RESULT and REG_CONFIDENCE */
        
        /* TODO: Set STATUS_READY (0x03) when finished */
    }
}
