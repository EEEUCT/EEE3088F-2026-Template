import sys
import re

def verify_contract():
    path = "firmware/include/i2c_registers.h"
    try:
        with open(path, 'r') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: {path} not found.")
        return False

    # 1. Check Slave Address
    if not re.search(r'#define\s+SLAVE_ADDRESS\s+0x42', content):
        print("FAIL: SLAVE_ADDRESS must be 0x42")
        return False

    # 2. Check for Mandatory Struct Members (ensure no one renamed them)
    mandatory_members = ['status', 'discovery_freq', 'doa_result', 'confidence', 'student_id']
    for member in mandatory_members:
        if member not in content:
            print(f"FAIL: Missing mandatory struct member '{member}'")
            return False

    # 3. Check for the Packing Attribute (Crucial for memory alignment)
    if '__attribute__((packed))' not in content:
        print("FAIL: struct i2c_register_map_t must be __attribute__((packed))")
        return False

    print("PASS: I2C Register Contract is intact.")
    return True

if __name__ == "__main__":
    if not verify_contract():
        sys.exit(1) # Tells GitHub Actions the test failed
