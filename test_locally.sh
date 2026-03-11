#!/bin/bash

# 1. Run Python static analysis on the I2C register map
python3 firmware/tests/verify_registers.py && \

# 2. Compile and run the I2C Logic Audit
gcc -std=c11 \
    firmware/src/i2c_slave.c \
    firmware/tests/test_i2c_logic.c \
    -I firmware/include \
    -o firmware/tests/i2c_audit -lm && \
./firmware/tests/i2c_audit && \

# 3. Compile and run the DoA Engine Sanity Check
# Note: Added public_vectors.c and the -I for the data folder
gcc -std=c11 \
    firmware/src/doa_engine.c \
    firmware/tests/test_doa.c \
    firmware/tests/data/public_vectors.c \
    -I firmware/include \
    -I firmware/tests/data \
    -o firmware/tests/test_doa -lm && \
./firmware/tests/test_doa
