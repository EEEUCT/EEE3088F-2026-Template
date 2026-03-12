import numpy as np
import argparse
import os

# -----------------------------------------------------------------------------
# STUDENT NOTE:
# This script is your "Signal Generator" for the DoA project.
# It uses Python (which has perfect floating-point math) to create 
# "Ground Truth" sine waves that are delayed by the exact amount of time
# sound takes to travel between two microphones at a specific angle.
# 
# If your C code disagrees with these vectors, your C code is wrong.
# -----------------------------------------------------------------------------

# --- Rig Constants ---
FS = 100000           # 100 kSPS (Sample Rate)
F_TONE = 4000.0       # 4 kHz Tone (Target Frequency)
D_MM = 40.0           # 40mm spacing (Distance between mics)
C_SPEED = 343.0       # Speed of sound (m/s) at 20C
N_SAMPLES = 500       # 5ms of data (buffer size)

def generate_signal_pair(angle_deg, snr_db=None, dc_offset=0, amplitude=2000):
    """
    Generates a pair of delayed sine waves with optional noise/offset.
    
    The Physics:
    Sound arriving from an angle theta travels an extra distance 'd * sin(theta)'
    to reach the further microphone.
    Time Delay (tau) = Extra Distance / Speed of Sound
    """
    t = np.arange(N_SAMPLES) / FS
    angle_rad = np.radians(angle_deg)
    
    # Calculate time delay (tau)
    tau = (D_MM / 1000.0 * np.sin(angle_rad)) / C_SPEED
    
    # Generate clean 4kHz signals
    # mic_left is the reference (t)
    # mic_right is the delayed version (t - tau)
    left = np.sin(2 * np.pi * F_TONE * t)
    right = np.sin(2 * np.pi * F_TONE * (t - tau))
    
    # Add noise if requested (The 'Separator' for good vs bad students)
    # Real world signals are never perfect. We add Gaussian noise to simulate
    # thermal noise and ADC quantization error.
    if snr_db is not None:
        p_sig = 0.5 # Power of a sine wave
        p_noise = p_sig / (10 ** (snr_db / 10.0))
        noise = np.random.normal(0, np.sqrt(p_noise), N_SAMPLES)
        left += noise
        right += noise

    # Scale to 16-bit integer range (simulate ADC values with specific amplitude)
    # Default 2000 is "Ideal High Gain". 250 is "Realistic 67dB SPL".
    # The ADC is 12-bit usually, but stored in 16-bit container.
    left_int = ((left + dc_offset) * amplitude).astype(np.int16)
    right_int = (right * amplitude).astype(np.int16)
    
    return left_int, right_int

def write_c_files(test_cases, folder_path, is_secret_mode):
    """Writes the results into public_vectors.c and .h"""
    h_path = os.path.join(folder_path, "public_vectors.h")
    c_path = os.path.join(folder_path, "public_vectors.c")

    with open(h_path, 'w') as h, open(c_path, 'w') as c:
        # Write Header Preamble
        h.write("/*\n")
        h.write(" * PUBLIC_VECTORS.H\n")
        h.write(" * \n")
        h.write(" * This file contains the 'Answer Key' for the DoA project.\n")
        h.write(" * It exports a registry of test cases (struct array) for the test harness.\n")
        h.write(" * \n")
        h.write(" * Usage:\n")
        h.write(" * Loop through 'test_registry' and run each case.\n")
        h.write(" */\n\n")

        # Write Header Guard
        h.write("#ifndef PUBLIC_VECTORS_H\n#define PUBLIC_VECTORS_H\n\n#include <stdint.h>\n\n")
        
        # Write Metadata Constants
        h.write("/* Rig Configuration Constants */\n")
        h.write(f"#define PUBLIC_VECTORS_FS   {FS}    // Sample Rate (Hz)\n")
        h.write(f"#define PUBLIC_VECTORS_N    {N_SAMPLES}      // Buffer Length (Samples)\n")
        h.write(f"#define PUBLIC_VECTORS_D_MM {D_MM}     // Microphone Spacing (mm)\n\n")

        # Write Registry Struct Definition
        h.write("typedef struct {\n")
        h.write("    const char* name;\n")
        h.write("    const int16_t* mic_left;\n")
        h.write("    const int16_t* mic_right;\n")
        h.write("    int16_t expected_angle;\n")
        h.write("    int is_hidden;\n")
        h.write("} doa_test_vector_t;\n\n")
        
        h.write("extern const doa_test_vector_t test_registry[];\n")
        h.write("extern const int test_registry_count;\n\n")

        c.write("/*\n")
        c.write(" * PUBLIC_VECTORS.C\n")
        c.write(" * \n")
        c.write(" * Raw integer data representing microphone ADC captures.\n")
        c.write(" * Range: +/- 2000 (Simulating a 12-bit ADC signal)\n")
        c.write(" */\n\n")
        c.write('#include "public_vectors.h"\n\n')

        for angle, name, snr, dc, amp in test_cases:
            l, r = generate_signal_pair(angle, snr, dc, amp)
            
            # Check if this case should be commented out in the generated output
            prefix = ""
            if name in ["test_85_deg", "test_minus_70_deg", "test_45_deg_noisy"]:
                prefix = "// "
            
            # Add to Header
            h.write(f"/* Test Case: {angle} Degrees */\n")
            h.write(f"{prefix}extern const int16_t {name}_mic_left[{N_SAMPLES}];\n")
            h.write(f"{prefix}extern const int16_t {name}_mic_right[{N_SAMPLES}];\n\n")
            
            # Add to Source
            c.write(f"/* Test Case: {angle} Degrees */\n")
            c.write(f"{prefix}const int16_t {name}_mic_left[{N_SAMPLES}] = {{{', '.join(map(str, l))}}};\n")
            c.write(f"{prefix}const int16_t {name}_mic_right[{N_SAMPLES}] = {{{', '.join(map(str, r))}}};\n\n")

        h.write("\n#endif\n")
        
        # Write the Registry Array
        c.write("const doa_test_vector_t test_registry[] = {\n")
        for angle, name, snr, dc, amp in test_cases:
            if name in ["test_85_deg", "test_minus_70_deg", "test_45_deg_noisy"]:
                continue # Skip commented out cases
                
            is_hidden = 1 if "test_secret" in name else 0
            c.write(f"    {{ \"{name}\", {name}_mic_left, {name}_mic_right, {int(round(angle))}, {is_hidden} }},\n")
        c.write("};\n\n")
        
        c.write(f"const int test_registry_count = {len(test_cases) - 3};\n") # -3 for skipped

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--randomize", action="store_true", help="Generate secret stress-test data")
    args = parser.parse_args()

    # Target folder relative to script location
    data_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

    # TIER 1/2/3: Standard Public Sanity Kit
    # Format: (Angle, Name, SNR, DC_Offset, Amplitude)
    cases = [
        (0, "test_0_deg", None, 0, 2000),
        (30, "test_30_deg", None, 0, 2000),
        (-45, "test_minus_45_deg", None, 0, 2000),
        (0, "test_0_deg_noisy", 10, 0, 2000),     # Robustness (10dB)
        (0, "test_0_deg_quiet", 20, 0, 250),      # 67dB SPL Simulation (Low Amplitude)
        (85, "test_85_deg", None, 0, 2000),       # Edge Case: Near +90 limit
        (-70, "test_minus_70_deg", None, 0, 2000),# Edge Case: Large negative
        (45, "test_45_deg_noisy", 10, 0, 2000)    # Robustness: Noisy shifted angle
    ]

    if args.randomize:
        # TIER 4/5: Secret Sweep (Range -70 to 70 in steps of 10)
        print("Generating Full Angle Sweep (Clean & Noisy)...")

        # Physics constraint: Snap angles to nearest integer lag to avoid quantization failures
        # k = fs * d * sin(theta) / c
        for angle in range(-70, 71, 10):
            # 1. Calculate the physically solvable angle closest to the target
            rads = np.radians(angle)
            exact_lag = (FS * D_MM / 1000.0) * np.sin(rads) / C_SPEED
            integer_lag = round(exact_lag)
            solvable_angle = np.degrees(np.arcsin( (integer_lag * C_SPEED) / (FS * D_MM / 1000.0) ))
            
            # 2. Use this solvable angle for the test case name
            suffix = f"{abs(int(round(solvable_angle)))}"
            if solvable_angle < 0: suffix = f"minus_{suffix}"

            # 1. Clean (High SNR, High Amp)
            cases.append((solvable_angle, f"test_secret_{suffix}_clean", None, 0, 2000))
            
            # 2. Noisy (10dB SNR, High Amp)
            # Make the test slightly more forgiving at extreme angles where aliasing is a risk
            snr = 10
            if abs(angle) > 55: # Higher angles have 1/cos(theta) error amplification
                snr = 25        # Boost SNR to prevent catastrophic outliers at edges
            cases.append((solvable_angle, f"test_secret_{suffix}_noisy", snr, 0, 2000))
    else:
        print("Generating Public Sanity Kit only...")

    write_c_files(cases, data_dir, args.randomize)