#ifndef PUBLIC_VECTORS_H
#define PUBLIC_VECTORS_H

#include <stdint.h>

/**
 * EEE3088F Public Sanity Kit
 * -------------------------
 * These vectors are generated for:
 * Frequency (f): 4000 Hz
 * Sample Rate (fs): 100,000 Hz
 * Sample Count (N): 500 (5ms window)
 * Mic Spacing (d): 40.0 mm
 */

// --- TEST CASE 1: 0 Degrees (Broadside) ---
// Left and Right channels are identical. Output should be exactly 0.
extern const int16_t test_0_deg_mic_left[500];
extern const int16_t test_0_deg_mic_right[500];

// --- TEST CASE 2: +30 Degrees (Right) ---
// Right channel lags Left. Output should be approx +30.
extern const int16_t test_30_deg_mic_left[500];
extern const int16_t test_30_deg_mic_right[500];

// --- TEST CASE 3: -45 Degrees (Left) ---
// Left channel lags Right. Output should be approx -45.
extern const int16_t test_minus_45_deg_mic_left[500];
extern const int16_t test_minus_45_deg_mic_right[500];

#endif
