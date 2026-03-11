#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "doa_engine.h"
#include "public_vectors.h"

// Helper to run individual cases
int run_sanity_check(const char* name, const int16_t* l, const int16_t* r, int8_t expected) {
    size_t N = 500;
    float d_mm = 40.0;
    uint32_t fs = 100000;
    
    int8_t result = calculate_doa_angle_2mic(l, r, N, d_mm, fs);
    
    printf("[%s] Expected: %d, Got: %d ", name, expected, result);
    
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
    int errors = 0;

    errors += run_sanity_check("0 DEG ", test_0_deg_mic_left, test_0_deg_mic_right, 0);
    errors += run_sanity_check("30 DEG", test_30_deg_mic_left, test_30_deg_mic_right, 30);
    errors += run_sanity_check("-45 DEG", test_minus_45_deg_mic_left, test_minus_45_deg_mic_right, -45);

    if (errors == 0) {
        printf("\nRESULT: ALL PUBLIC TESTS PASSED. Ready for GitHub push.\n");
        return 0;
    } else {
        printf("\nRESULT: %d TEST(S) FAILED.\n", errors);
        return 1;
    }
}
