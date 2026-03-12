@echo off
REM -----------------------------------------------------------------------------
REM Local Test Runner (Windows CMD)
REM Run this script to verify your code before pushing to GitHub.
REM Usage: test_locally.bat
REM -----------------------------------------------------------------------------

echo --- 1. VERIFYING I2C REGISTERS ---
python firmware\tests\verify_registers.py
if %errorlevel% neq 0 (
    echo ^>^>^> FAIL: I2C Register Contract broken.
    exit /b 1
)

echo.
echo --- 2. COMPILING & RUNNING I2C LOGIC TEST ---
gcc -std=c11 firmware\src\i2c_slave.c firmware\src\doa_engine.c firmware\tests\test_i2c_logic.c -I firmware\include -o firmware\tests\i2c_audit.exe -lm
if %errorlevel% neq 0 (
    echo ^>^>^> FAIL: Compilation Error (I2C Logic)
    exit /b 1
)

firmware\tests\i2c_audit.exe
if %errorlevel% neq 0 exit /b 1

echo.
echo --- 3. COMPILING & RUNNING DoA ENGINE TEST ---
gcc -std=c11 firmware\src\doa_engine.c firmware\tests\test_doa.c firmware\tests\data\public_vectors.c -I firmware\include -I firmware\tests\data -o firmware\tests\test_doa.exe -lm
if %errorlevel% neq 0 (
    echo ^>^>^> FAIL: Compilation Error (DoA Engine)
    exit /b 1
)

firmware\tests\test_doa.exe
if %errorlevel% neq 0 exit /b 1

REM Cleanup
del firmware\tests\i2c_audit.exe firmware\tests\test_doa.exe