#include "doa_engine.h"

/**
 * Milestone 3: Direction of Arrival Implementation
 * * Students: Implement your DOA logic below. Your implementation should 
 * use the cross-correlation of mic_left_data and mic_right_data to 
 * determine the incident angle of the sound source.
 */
int8_t calculate_doa_angle_2mic(const int16_t* mic_left_data, const int16_t* mic_right_data, size_t N, float d_mm, uint32_t fs) {
    
    // TODO: Implement cross-correlation to find time lag
    // HINT: Limit your search window (max_lag) to the physical max delay possible.
    //       Searching too far might find "aliased" peaks if the signal is periodic!
    
    // TODO: Convert lag to angle based on microphone spacing (d_mm)
    // HINT: The argument to asin() must be between -1.0 and 1.0.
    //       Due to noise or rounding, your calculation might produce 1.001.
    //       Clamp this value before calling asin() to avoid NaN errors.
    
    // TODO: Return angle in degrees (-90 to 90)

    return 0; // Default return to allow compilation
}
