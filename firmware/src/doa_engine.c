#include "doa_engine.h"

/**
 * Milestone 3: Direction of Arrival Implementation
 * * Students: Implement your DOA logic below. Your implementation should 
 * use the cross-correlation of mic_left_data and mic_right_data to 
 * determine the incident angle of the sound source.
 */
int8_t calculate_doa_angle_2mic(const int16_t* mic_left_data, const int16_t* mic_right_data, size_t N, float d_mm, uint32_t fs) {
    
    // --- PERFORMANCE NOTE ---
    // Basic time-domain cross-correlation is computationally heavy O(N * max_lag).
    // If your STM32 exceeds the 50ms execution budget, you may need to explore alternatives:
    //   - Phase Difference (Frequency Domain): Use the Goertzel algorithm to efficiently 
    //     extract the phase of the specific 2.5kHz target frequency without a full FFT.
    //   - Zero-Crossing Detection: Very fast time-domain method, but highly sensitive to noise.
    //   - CMSIS-DSP: Use the highly optimized ARM math libraries for correlation.
    // ------------------------

    // TODO: Step 1 - Implement cross-correlation to find the time lag 'k'
    // HINT 1: Limit your search window (max_lag) to the physical max delay possible.
    //         Searching too far might find "aliased" peaks if the signal is periodic!
    // HINT 2: You will need an outer loop for the shift 'k' (-max_lag to +max_lag) 
    //         and an inner loop to calculate the sum of the product of the arrays.
    // HINT 3: BE CAREFUL WITH ARRAY BOUNDS! When shifting by 'k', ensure your inner 
    //         loop does not read past index 0 or index N-1.
    
    // TODO: Step 2 - Convert lag to angle based on microphone spacing (d_mm)
    // HINT 4: The argument to asin() must be between -1.0 and 1.0.
    //       Due to noise or rounding, your calculation might produce 1.001.
    //       Clamp this value before calling asin() to avoid NaN errors.
    
    // TODO: Return angle in degrees (-90 to 90)

    return 0; // Default return to allow compilation
}
