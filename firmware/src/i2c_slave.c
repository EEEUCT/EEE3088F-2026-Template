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
    if (reg_addr == REG_SYS_STATUS && data == STATUS_TRIG) {
        /* ATP-SW-03: Transition to BUSY immediately to acknowledge Rig Master */
        my_registers.status = STATUS_BUSY;

        /* * TODO: Signal your main loop that a measurement is requested.
         * Example: set a 'measurement_pending' flag to 1.
         */
        /* Note: The main loop polls 'my_registers.status' to detect this request. */
    }
}

/**
 * @brief Initializes the constant registers.
 * Called once at startup to satisfy ATP-SW-01 and ATP-SW-02.
 */
void init_registers(float contracted_ft, const char* my_id) {
    memset(&my_registers, 0, sizeof(i2c_register_map_t));

    /* Pre-load the Discovery constants from the M1 Contract */
    my_registers.discovery_freq = contracted_ft;
    
    /* Ensure Student ID is exactly 9 characters */
    strncpy(my_registers.student_id, my_id, 9);

    /* Initial state: System is ready for a Trigger */
    my_registers.status = STATUS_READY;
}

/**
 * @brief The Main Processing Loop (M3 DSP Logic)
 * * Requirement: Total execution time between STATUS_TRIG and STATUS_READY 
 * must be <= 50ms.
 */
void process_doa_update(float d_mm, uint32_t fs) {
    if (my_registers.status == STATUS_BUSY) {
        
        /* 1. Perform the DSP math defined in doa_engine.c */
        int8_t result = calculate_doa_angle_2mic(mic_L, mic_R, 1024, d_mm, fs);

        /* 2. Update the Memory Map with the results */
        my_registers.doa_result = result;
        my_registers.confidence = 100; // Static confidence (DoA engine does not return score)

        /* 3. Handshake: Signal to the Automated Rig that data is now valid */
        my_registers.status = STATUS_READY;
    }
}
