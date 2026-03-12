import subprocess
import re
import sys
import os
import platform
import argparse

def run_command(cmd, shell=True):
    """Runs a shell command and returns the result object."""
    return subprocess.run(cmd, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)

def main():
    print("------------------------------------------------")
    print("       EEE3088F FIRMWARE GRADING RUNNER         ")
    print("------------------------------------------------")

    parser = argparse.ArgumentParser()
    parser.add_argument("--secret", action="store_true", help="Run hidden tests")
    args = parser.parse_args()

    is_windows = platform.system() == "Windows"
    exe_ext = ".exe" if is_windows else ""
    
    # --- PHASE 1: I2C TESTS (30%) ---
    print("\n[PHASE 1] I2C COMPLIANCE (30%)")
    i2c_points = 0
    i2c_max_points = 0
    public_i2c_passed = False

    # 1. Static Contract Check
    ret_static = run_command(f"{sys.executable} firmware/tests/verify_registers.py")
    if ret_static.returncode != 0:
        print(">>> FAIL: I2C Register Contract broken.")
        print(ret_static.stdout)
    else:
        print(">>> PASS: Register Contract Intact.")
        
        # 2. Logic State Machine Check
        compile_flags = ""
        if args.secret:
            compile_flags = "-DAUTOGRADER_I2C_STRESS"

        # Compile
        cmd_compile_i2c = (
            f"gcc -std=c11 {compile_flags} firmware/src/i2c_slave.c firmware/src/doa_engine.c "
            f"firmware/tests/test_i2c_logic.c -I firmware/include "
            f"-o i2c_audit{exe_ext} -lm"
        )
        ret_compile = run_command(cmd_compile_i2c)
        
        if ret_compile.returncode != 0:
            print(">>> FAIL: I2C Compilation Error")
            print(ret_compile.stdout)
        else:
            # Run
            ret_logic = run_command(f".{os.sep}i2c_audit{exe_ext}")
            print(ret_logic.stdout.strip())
            
            # Interpret bitmask: bit 0 = public fail, bit 1 = hidden fail
            public_i2c_passed = (ret_logic.returncode & 1) == 0
            hidden_i2c_passed = (ret_logic.returncode & 2) == 0

            i2c_max_points = 15 # Public part is always tested
            if public_i2c_passed:
                i2c_points += 15

            if args.secret:
                i2c_max_points += 15
                if hidden_i2c_passed:
                    i2c_points += 15
            
            print(f">>> I2C SCORE: {i2c_points} / {i2c_max_points}")

    # --- PHASE 2: DoA TESTS (70%) ---
    print("\n[PHASE 2] DoA ESTIMATION (70%)")
    doa_percentage = 0.0
    doa_raw = 0
    doa_max = 0
    
    # Compile
    cmd_compile_doa = (
        f"gcc -std=c11 firmware/src/doa_engine.c firmware/tests/test_doa.c "
        f"firmware/tests/data/public_vectors.c -I firmware/include -I firmware/tests/data "
        f" -o test_doa{exe_ext} -lm"
    )
    ret_compile_doa = run_command(cmd_compile_doa)

    if ret_compile_doa.returncode != 0:
        print(">>> FAIL: DoA Compilation Error")
        print(ret_compile_doa.stdout)
    else:
        # Run
        ret_doa = run_command(f".{os.sep}test_doa{exe_ext}")
        print(ret_doa.stdout.strip())
        
        # Parse "RAW DOA SCORE : X / Y"
        match = re.search(r"RAW DOA SCORE\s*:\s*(\d+)\s*/\s*(\d+)", ret_doa.stdout)
        if match:
            doa_raw = int(match.group(1))
            doa_max = int(match.group(2))
            if doa_max > 0:
                doa_percentage = (doa_raw / doa_max) * 70.0
        else:
            # If we don't see the score line, check if public tests failed (which usually exits early)
            # However, for partial marks, we want to know what happened.
            # In local mode, test_doa might not print "RAW DOA SCORE" if hidden tests aren't enabled.
            if "ALL PUBLIC TESTS PASSED" in ret_doa.stdout:
                print("(Local Mode: DoA Passing)")
            
    # --- FINAL SCORE ---
    i2c_percentage = 0
    if i2c_max_points > 0:
        i2c_percentage = (i2c_points / i2c_max_points) * 30.0

    final_score = i2c_percentage + doa_percentage
    final_int = int(round(final_score))
    
    print("\n======================================================")
    print(f"TOTAL MARKS : {final_int} / 100")
    print("======================================================")

    # Cleanup
    for f in [f"i2c_audit{exe_ext}", f"test_doa{exe_ext}"]:
        if os.path.exists(f):
            os.remove(f)

    # Return failure if I2C failed OR DoA Public tests failed (Hard Gates)
    # We want the Red X if the basics aren't working, even if they got some points.
    if not public_i2c_passed or (doa_max > 0 and doa_raw == 0):
        sys.exit(1)
    
if __name__ == "__main__":
    main()