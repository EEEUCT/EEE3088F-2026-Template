#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "doa_engine.h"
#include "public_vectors.h"

// Helper to calculate points: 10 points max, -1 point per degree of error
int calculate_points(int error) {
    if (error > 10) return 0;
    return 10 - error;
}

// Helper to run individual cases. Returns absolute error.
int run_sanity_check(const char* name, const int16_t* l, const int16_t* r, int8_t expected, int* points_out) {
    size_t N = PUBLIC_VECTORS_N;
    float d_mm = PUBLIC_VECTORS_D_MM;
    uint32_t fs = PUBLIC_VECTORS_FS;
    
    clock_t start = clock();
    int8_t result = calculate_doa_angle_2mic(l, r, N, d_mm, fs);
    clock_t end = clock();
    double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    
    int error = 0;
    
    // Check for error code
    if (result == -128) {
        printf("[%s] Expected: %3d, Got: ERR (Time: %.3f ms) -> FAIL (Error Code -128)\n", name, expected, time_ms);
        *points_out = 0;
        return 999; // Return large error
    }

    error = abs(result - expected);
    *points_out = calculate_points(error);

    printf("[%-25s] Expected: %3d, Got: %3d (Err: %2d deg) ", name, expected, result, error);

    // Tolerance of +/- 2 degrees for clean synthetic data
    if (error <= 2) {
        printf("-> PASS (+%d pts)\n", *points_out);
        return 0;
    } else {
        printf("-> FAIL (+%d pts)\n", *points_out);
        return error; // Return error magnitude (non-zero is fail for strict count)
    }
}

int main() {
    printf("--- EEE3088F DoA Engine Sanity Check ---\n");
    
    int public_errors = 0;
    int public_count = 0;
    
    int secret_errors = 0;
    int secret_count = 0;
    
    int total_score = 0;
    int max_score = 0;
    int pts = 0;
    
    // Advisor Statistics
    int noise_fails = 0;
    int edge_fails = 0;
    int center_fails = 0;
    int total_noise_cases = 0;
    int total_edge_cases = 0;
    int robustness_fail = 0;

    printf("--- REGISTRY TESTS ---\n");
    
    // Dynamic Registry Loop (Handles Public + Secret Tests automatically)
    for (int i = 0; i < test_registry_count; i++) {
        doa_test_vector_t t = test_registry[i];
        
        // Classify Test Case based on name and angle
        int is_noisy = (strstr(t.name, "noisy") != NULL || strstr(t.name, "quiet") != NULL);
        int is_edge = (abs(t.expected_angle) >= 60);
        int is_center = (abs(t.expected_angle) <= 15);

        if (is_noisy) total_noise_cases++;
        if (is_edge) total_edge_cases++;

        int fail_magnitude = run_sanity_check(t.name, t.mic_left, t.mic_right, t.expected_angle, &pts);
        
        // Track significant failures for the advisor (>5 deg error suggests algorithmic issue)
        if (fail_magnitude > 5) {
            if (is_noisy) noise_fails++;
            if (is_edge) edge_fails++;
            if (is_center) center_fails++;
        }

        total_score += pts;
        max_score += 10;

        if (t.is_hidden) {
            secret_count++;
            if (fail_magnitude > 2) secret_errors++;
        } else {
            public_count++;
            if (fail_magnitude > 2) public_errors++;
        }
    }

    printf("--- ROBUSTNESS CHECKS ---\n");
    public_count++;
    max_score += 10;
    if (calculate_doa_angle_2mic(NULL, NULL, PUBLIC_VECTORS_N, PUBLIC_VECTORS_D_MM, PUBLIC_VECTORS_FS) != -128) {
         printf("FAIL: System did not handle NULL pointers safely (Expected -128).\n");
         public_errors++;
         robustness_fail = 1;
         // 0 points
    } else {
         printf("PASS: System handled NULL pointers safely. (+10 pts)\n");
         total_score += 10;
    }

    // --- GRADING & SUMMARY ---
    printf("\n======================================================\n");
    printf("                AUTOGRADER SUMMARY\n");
    printf("======================================================\n");
    printf("PUBLIC TESTS  : %d / %d Passed\n", public_count - public_errors, public_count);
    
    if (secret_count > 0) {
        printf("HIDDEN TESTS  : %d / %d Passed\n", secret_count - secret_errors, secret_count);
        printf("------------------------------------------------------\n");
        printf("RAW DOA SCORE : %d / %d\n", total_score, max_score);
    } else {
        printf("(Hidden tests skipped - Local Mode)\n");
    }
    printf("======================================================\n");

    // --- ADVISOR FEEDBACK ---
    // Only show advice if there are failures (public or secret)
    if (public_errors > 0 || secret_errors > 0) {
        printf("\n>>> 💡 DoA ALGORITHM ADVICE <<<\n");
        
        if (robustness_fail) {
            printf("(!) ROBUSTNESS: Your code crashed or didn't return -128 for NULL inputs.\n");
            printf("    Always validate your pointers at the very start of the function.\n");
        }
        else if (center_fails > 0) {
            printf("(!) BASIC MATH: You are failing tests near 0 degrees (Boresight).\n");
            printf("    Check your lag-to-angle formula and index calculations.\n");
        }
        else if (edge_fails > (total_edge_cases / 2)) {
            printf("(!) ENDFIRE ISSUES: High error rates at large angles (>60 deg).\n");
            printf("    The derivative of asin(x) explodes near x=1, magnifying small timing errors.\n");
            printf("    Ensure you clamp the argument to [-1.0, 1.0] before calling asin().\n");
        }
        
        if (noise_fails > (total_noise_cases / 2)) {
            printf("(!) NOISE SENSITIVITY: Your algorithm is struggling with noisy signals.\n");
            printf("    Consider averaging multiple correlation windows or pre-filtering the data.\n");
        }
    }

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
