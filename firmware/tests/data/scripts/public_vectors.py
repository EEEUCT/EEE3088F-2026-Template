import numpy as np
import argparse
import os

# --- Rig Constants ---
FS = 100000           # 100 kSPS
F_TONE = 4000.0       # 4 kHz
D_MM = 40.0           # 40mm spacing
C_SPEED = 343.0       # Speed of sound (m/s)
N_SAMPLES = 500       # 5ms of data

def generate_signal_pair(angle_deg, snr_db=None, dc_offset=0):
    """Generates a pair of delayed sine waves with optional noise/offset."""
    t = np.arange(N_SAMPLES) / FS
    angle_rad = np.radians(angle_deg)
    
    # Calculate time delay (tau)
    tau = (D_MM / 1000.0 * np.sin(angle_rad)) / C_SPEED
    
    # Generate clean 4kHz signals
    left = np.sin(2 * np.pi * F_TONE * t)
    right = np.sin(2 * np.pi * F_TONE * (t - tau))
    
    # Add noise if requested (The 'Separator' for good vs bad students)
    if snr_db is not None:
        p_sig = 0.5 # Power of a sine wave
        p_noise = p_sig / (10 ** (snr_db / 10.0))
        noise = np.random.normal(0, np.sqrt(p_noise), N_SAMPLES)
        left += noise
        right += noise

    # Scale to 16-bit integer range (simulate ADC values)
    # We use a max amplitude of 2000 to leave headroom for noise/offset
    left_int = ((left + dc_offset) * 2000).astype(np.int16)
    right_int = (right * 2000).astype(np.int16)
    
    return left_int, right_int

def write_c_files(test_cases, folder_path):
    """Writes the results into public_vectors.c and .h"""
    h_path = os.path.join(folder_path, "public_vectors.h")
    c_path = os.path.join(folder_path, "public_vectors.c")

    with open(h_path, 'w') as h, open(c_path, 'w') as c:
        # Write Header Guard
        h.write("#ifndef PUBLIC_VECTORS_H\n#define PUBLIC_VECTORS_H\n\n#include <stdint.h>\n\n")
        c.write('#include "public_vectors.h"\n\n')

        for angle, name, snr, dc in test_cases:
            l, r = generate_signal_pair(angle, snr, dc)
            
            # Add to Header
            h.write(f"extern const int16_t {name}_mic_left[{N_SAMPLES}];\n")
            h.write(f"extern const int16_t {name}_mic_right[{N_SAMPLES}];\n")
            
            # Add to Source
            c.write(f"const int16_t {name}_mic_left[{N_SAMPLES}] = {{{', '.join(map(str, l))}}};\n")
            c.write(f"const int16_t {name}_mic_right[{N_SAMPLES}] = {{{', '.join(map(str, r))}}};\n\n")

        h.write("\n#endif\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--randomize", action="store_true", help="Generate secret stress-test data")
    args = parser.parse_args()

    # Target folder relative to script location
    data_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

    if args.randomize:
        # TIER 4/5: Secret randomized test case
        # Generates a random angle with noise and DC offset to prevent hardcoding
        rand_angle = np.random.uniform(-80, 80)
        cases = [(rand_angle, "test_secret", 15, 0.1)] # 15dB SNR, 10% DC offset
        print(f"Generating Randomized Stress Test at {rand_angle:.2f} degrees...")
    else:
        # TIER 1/2/3: Standard Public Sanity Kit
        cases = [
            (0, "test_0_deg", None, 0),
            (30, "test_30_deg", None, 0),
            (-45, "test_minus_45_deg", None, 0)
        ]
        print("Generating Public Sanity Kit (0, 30, -45 degrees)...")

    write_c_files(cases, data_dir)