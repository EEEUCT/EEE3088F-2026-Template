/*
 * PUBLIC_VECTORS.H
 * 
 * This file contains the 'Answer Key' for the DoA project.
 * It exports a registry of test cases (struct array) for the test harness.
 * 
 * Usage:
 * Loop through 'test_registry' and run each case.
 */

#ifndef PUBLIC_VECTORS_H
#define PUBLIC_VECTORS_H

#include <stdint.h>

/* Rig Configuration Constants */
#define PUBLIC_VECTORS_FS   100000    // Sample Rate (Hz)
#define PUBLIC_VECTORS_N    500      // Buffer Length (Samples)
#define PUBLIC_VECTORS_D_MM 40.0     // Microphone Spacing (mm)

typedef struct {
    const char* name;
    const int16_t* mic_left;
    const int16_t* mic_right;
    int16_t expected_angle;
    int is_hidden;
} doa_test_vector_t;

extern const doa_test_vector_t test_registry[];
extern const int test_registry_count;

/* Test Case: 0 Degrees */
extern const int16_t test_0_deg_mic_left[500];
extern const int16_t test_0_deg_mic_right[500];

/* Test Case: 30 Degrees */
extern const int16_t test_30_deg_mic_left[500];
extern const int16_t test_30_deg_mic_right[500];

/* Test Case: -45 Degrees */
extern const int16_t test_minus_45_deg_mic_left[500];
extern const int16_t test_minus_45_deg_mic_right[500];

/* Test Case: 0 Degrees */
extern const int16_t test_0_deg_noisy_mic_left[500];
extern const int16_t test_0_deg_noisy_mic_right[500];

/* Test Case: 0 Degrees */
extern const int16_t test_0_deg_quiet_mic_left[500];
extern const int16_t test_0_deg_quiet_mic_right[500];

/* Test Case: 85 Degrees */
// extern const int16_t test_85_deg_mic_left[500];
// extern const int16_t test_85_deg_mic_right[500];

/* Test Case: -70 Degrees */
// extern const int16_t test_minus_70_deg_mic_left[500];
// extern const int16_t test_minus_70_deg_mic_right[500];

/* Test Case: 45 Degrees */
// extern const int16_t test_45_deg_noisy_mic_left[500];
// extern const int16_t test_45_deg_noisy_mic_right[500];

/* Test Case: -70.60471688946346 Degrees */
extern const int16_t test_secret_minus_71_clean_mic_left[500];
extern const int16_t test_secret_minus_71_clean_mic_right[500];

/* Test Case: -70.60471688946346 Degrees */
extern const int16_t test_secret_minus_71_noisy_mic_left[500];
extern const int16_t test_secret_minus_71_noisy_mic_right[500];

/* Test Case: -59.037031290672296 Degrees */
extern const int16_t test_secret_minus_59_clean_mic_left[500];
extern const int16_t test_secret_minus_59_clean_mic_right[500];

/* Test Case: -59.037031290672296 Degrees */
extern const int16_t test_secret_minus_59_noisy_mic_left[500];
extern const int16_t test_secret_minus_59_noisy_mic_right[500];

/* Test Case: -50.511298456656085 Degrees */
extern const int16_t test_secret_minus_51_clean_mic_left[500];
extern const int16_t test_secret_minus_51_clean_mic_right[500];

/* Test Case: -50.511298456656085 Degrees */
extern const int16_t test_secret_minus_51_noisy_mic_left[500];
extern const int16_t test_secret_minus_51_noisy_mic_right[500];

/* Test Case: -36.887804675959465 Degrees */
extern const int16_t test_secret_minus_37_clean_mic_left[500];
extern const int16_t test_secret_minus_37_clean_mic_right[500];

/* Test Case: -36.887804675959465 Degrees */
extern const int16_t test_secret_minus_37_noisy_mic_left[500];
extern const int16_t test_secret_minus_37_noisy_mic_right[500];

/* Test Case: -30.96404014543338 Degrees */
extern const int16_t test_secret_minus_31_clean_mic_left[500];
extern const int16_t test_secret_minus_31_clean_mic_right[500];

/* Test Case: -30.96404014543338 Degrees */
extern const int16_t test_secret_minus_31_noisy_mic_left[500];
extern const int16_t test_secret_minus_31_noisy_mic_right[500];

/* Test Case: -20.059756050635414 Degrees */
extern const int16_t test_secret_minus_20_clean_mic_left[500];
extern const int16_t test_secret_minus_20_clean_mic_right[500];

/* Test Case: -20.059756050635414 Degrees */
extern const int16_t test_secret_minus_20_noisy_mic_left[500];
extern const int16_t test_secret_minus_20_noisy_mic_right[500];

/* Test Case: -9.875043682290848 Degrees */
extern const int16_t test_secret_minus_10_clean_mic_left[500];
extern const int16_t test_secret_minus_10_clean_mic_right[500];

/* Test Case: -9.875043682290848 Degrees */
extern const int16_t test_secret_minus_10_noisy_mic_left[500];
extern const int16_t test_secret_minus_10_noisy_mic_right[500];

/* Test Case: 0.0 Degrees */
extern const int16_t test_secret_0_clean_mic_left[500];
extern const int16_t test_secret_0_clean_mic_right[500];

/* Test Case: 0.0 Degrees */
extern const int16_t test_secret_0_noisy_mic_left[500];
extern const int16_t test_secret_0_noisy_mic_right[500];

/* Test Case: 9.875043682290848 Degrees */
extern const int16_t test_secret_10_clean_mic_left[500];
extern const int16_t test_secret_10_clean_mic_right[500];

/* Test Case: 9.875043682290848 Degrees */
extern const int16_t test_secret_10_noisy_mic_left[500];
extern const int16_t test_secret_10_noisy_mic_right[500];

/* Test Case: 20.059756050635414 Degrees */
extern const int16_t test_secret_20_clean_mic_left[500];
extern const int16_t test_secret_20_clean_mic_right[500];

/* Test Case: 20.059756050635414 Degrees */
extern const int16_t test_secret_20_noisy_mic_left[500];
extern const int16_t test_secret_20_noisy_mic_right[500];

/* Test Case: 30.96404014543338 Degrees */
extern const int16_t test_secret_31_clean_mic_left[500];
extern const int16_t test_secret_31_clean_mic_right[500];

/* Test Case: 30.96404014543338 Degrees */
extern const int16_t test_secret_31_noisy_mic_left[500];
extern const int16_t test_secret_31_noisy_mic_right[500];

/* Test Case: 36.887804675959465 Degrees */
extern const int16_t test_secret_37_clean_mic_left[500];
extern const int16_t test_secret_37_clean_mic_right[500];

/* Test Case: 36.887804675959465 Degrees */
extern const int16_t test_secret_37_noisy_mic_left[500];
extern const int16_t test_secret_37_noisy_mic_right[500];

/* Test Case: 50.511298456656085 Degrees */
extern const int16_t test_secret_51_clean_mic_left[500];
extern const int16_t test_secret_51_clean_mic_right[500];

/* Test Case: 50.511298456656085 Degrees */
extern const int16_t test_secret_51_noisy_mic_left[500];
extern const int16_t test_secret_51_noisy_mic_right[500];

/* Test Case: 59.037031290672296 Degrees */
extern const int16_t test_secret_59_clean_mic_left[500];
extern const int16_t test_secret_59_clean_mic_right[500];

/* Test Case: 59.037031290672296 Degrees */
extern const int16_t test_secret_59_noisy_mic_left[500];
extern const int16_t test_secret_59_noisy_mic_right[500];

/* Test Case: 70.60471688946346 Degrees */
extern const int16_t test_secret_71_clean_mic_left[500];
extern const int16_t test_secret_71_clean_mic_right[500];

/* Test Case: 70.60471688946346 Degrees */
extern const int16_t test_secret_71_noisy_mic_left[500];
extern const int16_t test_secret_71_noisy_mic_right[500];


#endif
