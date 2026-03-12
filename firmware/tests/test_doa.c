#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "doa_engine.h"
#include "public_vectors.h"

// Helper to run individual cases
int run_sanity_check(const char* name, const int16_t* l, const int16_t* r, int8_t expected) {
    size_t N = PUBLIC_VECTORS_N;
    float d_mm = PUBLIC_VECTORS_D_MM;
    uint32_t fs = PUBLIC_VECTORS_FS;
    
    clock_t start = clock();
    int8_t result = calculate_doa_angle_2mic(l, r, N, d_mm, fs);
    clock_t end = clock();
    double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    
    printf("[%s] Expected: %3d, Got: %3d (Time: %.3f ms) ", name, expected, result, time_ms);
    
    // Check for error code
    if (result == -128) {
        printf("-> FAIL (Error Code -128)\n");
        return 1;
    }

    // Tolerance of +/- 2 degrees for clean synthetic data
    if (abs(result - expected) <= 2) {
        printf("-> PASS\n");
        return 0;
    } else {
        printf("-> FAIL (Outside Tolerance)\n");
        return 1;
    }
}

int main() {
    printf("--- EEE3088F DoA Engine Sanity Check ---\n");
    
    int public_errors = 0;
    int public_count = 0;
    
    int secret_errors = 0;
    int secret_count = 0;

    // --- PUBLIC TESTS ---
    public_count++; public_errors += run_sanity_check("0 DEG ", test_0_deg_mic_left, test_0_deg_mic_right, 0);
    public_count++; public_errors += run_sanity_check("30 DEG", test_30_deg_mic_left, test_30_deg_mic_right, 30);
    public_count++; public_errors += run_sanity_check("-45 DEG", test_minus_45_deg_mic_left, test_minus_45_deg_mic_right, -45);
    
    // Noise Resilience Check (Crucial for M3)
    public_count++; public_errors += run_sanity_check("0 DEG (NOISY)", test_0_deg_noisy_mic_left, test_0_deg_noisy_mic_right, 0);
    
    // Low Signal Level Check (67 dB SPL Simulation)
    extern const int16_t test_0_deg_quiet_mic_left[];
    extern const int16_t test_0_deg_quiet_mic_right[];
    public_count++; public_errors += run_sanity_check("0 DEG (QUIET)", test_0_deg_quiet_mic_left, test_0_deg_quiet_mic_right, 0);

    printf("--- ROBUSTNESS CHECKS ---\n");
    public_count++;
    if (calculate_doa_angle_2mic(NULL, NULL, PUBLIC_VECTORS_N, PUBLIC_VECTORS_D_MM, PUBLIC_VECTORS_FS) != -128) {
         printf("FAIL: System did not handle NULL pointers safely (Expected -128).\n");
         public_errors++;
    } else {
         printf("PASS: System handled NULL pointers safely.\n");
    }

    /* 
     * AUTOGRADER HOOKS:
     * These tests only run on the GitHub Actions server.
     * They count towards the "Hidden/Bonus" mark.
     */
#ifdef AUTOGRADER_SECRET_ANGLE_BORE
    secret_count++;
    extern const int16_t test_secret_bore_mic_left[];
    extern const int16_t test_secret_bore_mic_right[];
    printf("--- RUNNING HIDDEN: BORESIGHT ACCURACY ---\n");
    secret_errors += run_sanity_check("SECRET BORE", test_secret_bore_mic_left, test_secret_bore_mic_right, AUTOGRADER_SECRET_ANGLE_BORE);
#endif

#ifdef AUTOGRADER_SECRET_ANGLE_END
    secret_count++;
    extern const int16_t test_secret_end_mic_left[];
    extern const int16_t test_secret_end_mic_right[];
    printf("--- RUNNING HIDDEN: ENDFIRE STABILITY ---\n");
    secret_errors += run_sanity_check("SECRET END", test_secret_end_mic_left, test_secret_end_mic_right, AUTOGRADER_SECRET_ANGLE_END);
#endif

#ifdef AUTOGRADER_SECRET_ANGLE_NOISE
    secret_count++;
    extern const int16_t test_secret_noise_mic_left[];
    extern const int16_t test_secret_noise_mic_right[];
    printf("--- RUNNING HIDDEN: NOISE SENSITIVITY ---\n");
    secret_errors += run_sanity_check("SECRET NOISE", test_secret_noise_mic_left, test_secret_noise_mic_right, AUTOGRADER_SECRET_ANGLE_NOISE);
#endif

#ifdef AUTOGRADER_SECRET_ANGLE_MODERATE
    secret_count++;
    extern const int16_t test_secret_moderate_mic_left[];
    extern const int16_t test_secret_moderate_mic_right[];
    printf("--- RUNNING HIDDEN: MODERATE ANGLE (NOISY) ---\n");
    secret_errors += run_sanity_check("SECRET MOD", test_secret_moderate_mic_left, test_secret_moderate_mic_right, AUTOGRADER_SECRET_ANGLE_MODERATE);
#endif

    // --- GRADING & SUMMARY ---
    printf("\n======================================================\n");
    printf("                AUTOGRADER SUMMARY\n");
    printf("======================================================\n");
    printf("PUBLIC TESTS  : %d / %d Passed\n", public_count - public_errors, public_count);
    
    if (secret_count > 0) {
        printf("HIDDEN TESTS  : %d / %d Passed\n", secret_count - secret_errors, secret_count);
        printf("------------------------------------------------------\n");
        printf("TOTAL MARKS   : %d / %d\n", (public_count + secret_count) - (public_errors + secret_errors), public_count + secret_count);
    } else {
        printf("(Hidden tests skipped - Local Mode)\n");
    }
    printf("======================================================\n");

    // GRADING POLICY:
    // 1. If Public Tests Pass -> Return 0 (GitHub Green Tick).
    // 2. If Public Tests Fail -> Return 1 (GitHub Red Cross).
    // 3. Hidden Tests -> Displayed in log for additional marks, but do not block the Green Tick.
    if (public_errors == 0) {
        printf("\n>>> SUCCESS: All Public Tests Passed. (CI Workflow Passing)\n");
        return 0;
    } else {
        printf("\n>>> FAILURE: Public Tests Failed. (CI Workflow Failing)\n");
        return 1;
    }
}
