import numpy as np

# Configuration
FS = 100000
F_TONE = 4000.0
D_MM = 40.0
C = 343.0
N = 500
t = np.arange(N) / FS

def get_arrays(angle_deg):
    tau = (D_MM/1000.0 * np.sin(np.radians(angle_deg))) / C
    # Mic Left is reference (0 delay), Mic Right is delayed by tau
    left = np.sin(2 * np.pi * F_TONE * t)
    right = np.sin(2 * np.pi * F_TONE * (t - tau))
    return (left * 2000).astype(np.int16), (right * 2000).astype(np.int16)

cases = [(0, "test_0_deg"), (30, "test_30_deg"), (-45, "test_minus_45_deg")]

with open("public_vectors.c", "w") as f:
    f.write('#include "public_vectors.h"\n\n')
    for angle, name in cases:
        l, r = get_arrays(angle)
        f.write(f"const int16_t {name}_mic_left[{N}] = {{{', '.join(map(str, l))}}};\n")
        f.write(f"const int16_t {name}_mic_right[{N}] = {{{', '.join(map(str, r))}}};\n\n")
