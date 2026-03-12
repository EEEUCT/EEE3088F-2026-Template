#!/bin/bash

# -----------------------------------------------------------------------------
# Local Test Runner
# Run this script to verify your code before pushing to GitHub.
# Usage: ./test_locally.sh
# -----------------------------------------------------------------------------

echo "--- 1. VERIFYING I2C REGISTERS ---"
python3 firmware/tests/verify_registers.py
if [ $? -ne 0 ]; then
    echo ">>> FAIL: I2C Register Contract broken."
    exit 1
fi

echo -e "\n--- 2. COMPILING & RUNNING I2C LOGIC TEST ---"
gcc -std=c11 \
    firmware/src/i2c_slave.c \
    firmware/src/doa_engine.c \
    firmware/tests/test_i2c_logic.c \
    -I firmware/include \
    -o firmware/tests/i2c_audit -lm

if [ $? -ne 0 ]; then
    echo ">>> FAIL: Compilation Error (I2C Logic)"
    exit 1
fi

./firmware/tests/i2c_audit
if [ $? -ne 0 ]; then exit 1; fi

echo -e "\n--- 3. COMPILING & RUNNING DoA ENGINE TEST ---"
gcc -std=c11 \
    firmware/src/doa_engine.c \
    firmware/tests/test_doa.c \
    firmware/tests/data/public_vectors.c \
    -I firmware/include \
    -I firmware/tests/data \
    -o firmware/tests/test_doa -lm

if [ $? -ne 0 ]; then
    echo ">>> FAIL: Compilation Error (DoA Engine)"
    exit 1
fi

./firmware/tests/test_doa
if [ $? -ne 0 ]; then exit 1; fi

rm -f firmware/tests/i2c_audit firmware/tests/test_doa firmware/tests/*.exe
