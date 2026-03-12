#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "i2c_registers.h"

// External references to the student's implementation
extern i2c_register_map_t my_registers;
extern void handle_i2c_write(uint8_t reg_addr, uint8_t data);
extern void init_registers(float contracted_ft, const char* my_id);
extern void process_doa_update(float d_mm, uint32_t fs);

/**
 * @brief Helper to print the status of the virtual rig
 */
void print_audit_log(const char* test_name, int passed) {
    printf("[AUDIT] %-30s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

int main() {
    printf("--- Starting Virtual Rig I2C Interface Audit ---\n\n");

    int errors = 0; // Use a bitmask for different failure types

    // --- TEST 1: Initial Discovery & ID (ATP-SW-01/02) ---
    float test_ft = 2500.0f;
    const char* test_id = "ABC123456";
    init_registers(test_ft, test_id);

    int id_ok = (strncmp(my_registers.student_id, test_id, 9) == 0);
    int ft_ok = (my_registers.discovery_freq == test_ft);
    int ready_ok = (my_registers.status == STATUS_READY);

    print_audit_log("ID Discovery (Reg 0x10)", id_ok);
    print_audit_log("Freq Discovery (Reg 0x04)", ft_ok);
    print_audit_log("Initial Status (Ready)", ready_ok);

    if (!id_ok || !ft_ok || !ready_ok) {
        errors |= 1; // Bit 0: Public test failure
    }

    // --- TEST 2: Trigger & State Transition (ATP-SW-03) ---
    // Simulate Master writing TRIG to SYS_STATUS
    handle_i2c_write(REG_SYS_STATUS, STATUS_TRIG);

    int busy_ok = (my_registers.status == STATUS_BUSY);
    print_audit_log("Transition to BUSY (Reg 0x00)", busy_ok);

    if (!busy_ok) {
        printf(">>> Error: System failed to enter BUSY state after Trigger.\n");
        errors |= 1;
    }

    // --- TEST 3: Handshake Completion ---
    // Simulate the main loop processing the data
    process_doa_update(50.0f, 48000);

    int back_to_ready = (my_registers.status == STATUS_READY);
    print_audit_log("Transition to READY (Done)", back_to_ready);

    if (!back_to_ready) {
        printf(">>> Error: System failed to return to READY after processing.\n");
        errors |= 1;
    }

    // --- [HIDDEN] TEST 4: Robustness to Spurious Commands ---
#ifdef AUTOGRADER_I2C_STRESS
    printf("\n--- [HIDDEN] I2C Stress Test ---\n");
    int stress_passed = 1;

    // Re-trigger to BUSY state
    handle_i2c_write(REG_SYS_STATUS, STATUS_TRIG);

    // Spurious write to another register. Should be ignored.
    handle_i2c_write(REG_CONFIDENCE, 50); // Write to confidence register
    if (my_registers.status != STATUS_BUSY) {
        print_audit_log("Ignore Spurious Write", 0);
        stress_passed = 0;
    } else {
        print_audit_log("Ignore Spurious Write", 1);
    }

    // Invalid write to status register. Should be ignored.
    handle_i2c_write(REG_SYS_STATUS, 0xFF); // Write invalid command
    if (my_registers.status != STATUS_BUSY) {
        print_audit_log("Ignore Invalid Trigger", 0);
        stress_passed = 0;
    } else {
        print_audit_log("Ignore Invalid Trigger", 1);
    }
    
    if (!stress_passed) {
        errors |= 2; // Bit 1: Hidden test failure
    }
#endif

    if (errors == 0) printf("\n--- All Interface Audits Passed Successfully ---\n");
    return errors;
}
