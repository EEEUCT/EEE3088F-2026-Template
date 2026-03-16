# EEE3088F: DoA Project Repository

This repository hosts the design, firmware, integration logic, and final forensic audit for the **Acoustic Direction of Arrival (DoA)** project. 

**Note:** This repository template is used across all milestones. Milestones 1 and 2 are **Group Submissions**. Milestones 3, 4, and 5 are **Individual Submissions**.

## 📂 Project Structure

* **`/analysis/`**: Python/MATLAB scripts for physics baselines, gain calculations, and SNR proofs.
* **`/hardware/`**: CAD project files (Schematic and PCB Layout).
* **`/hardware/production/`**: **The Manufacturing Gate.** All Gerbers, BOM, and CPL files for fabrication live here.
* **`/docs/`**: Datasheets for your selected MCU, Op-Amp, and Microphones.
* **`/firmware/`**: Source code (`/src`) and test harnesses (`/tests`) for the STM32F0.

## ⚖️ The "Hard Gates"

To ensure your board is accepted for manufacture, it must meet these three non-negotiable criteria:

1.  **Voltage:** Strictly **3.3V DC** (Power and I2C Logic). No 5V components.
2.  **Mechanical:** Mounting pitch must be exactly **50.0mm ± 0.2mm**.
3.  **Test Points:** All six mandatory TPs must be labeled and accessible (VCC, GND, SIGNAL, BIAS, SDA, SCL).

---

## 💻 Milestone 3: Firmware Implementation

Your task is to implement the Direction of Arrival (DoA) algorithm and the I2C communication logic.

### 1. Files to Edit
You should only modify these two files. **Do not modify the test harnesses, headers, or Python scripts.**
*   `firmware/src/doa_engine.c`: Implement your Cross-Correlation / Phase-Difference logic here.
*   `firmware/src/i2c_slave.c`: Implement the State Machine to handle Read/Write requests from the Master.

### 2. Local Testing
Before pushing to GitHub, you should verify your code locally. We have provided scripts that mimic the grading server.

**Mac/Linux:**
```bash
./test_locally.sh
```

**Windows:**
Double-click `test_locally.bat` or run it from CMD.

These scripts will:
1.  Check your I2C Register struct for compliance.
2.  Compile and test your I2C logic state machine.
3.  Compile and test your DoA Engine against **Public Test Vectors** (0°, 30°, -45°, Noise, Quiet).

**Note on Physics:** The provided public test vectors assume a **40.0mm** microphone spacing. The autograder will provide this value to your `calculate_doa_angle_2mic` function. Ensure your math uses the `d_mm` parameter provided to the function rather than a hardcoded constant.

### 3. Submission & Autograding (Hidden Tests)
When you push your code to GitHub, the **Actions** tab runs the "Firmware Autograder". This runs the public tests **PLUS** a set of **Hidden/Secret** tests.

### 📊 **Total Grade Breakdown**

| Module | Weight | Description |
| :--- | :--- | :--- |
| **Phase 1: I2C Compliance** | **30%** | Protocol handling, state machine logic, and register map compliance. |
| **Phase 2: DoA Engine** | **70%** | Signal processing accuracy, noise resilience, and robustness. |
| **Total** | **100%** | Combined weighted score. |

#### **1. I2C Compliance (30 Marks)**

This section is binary (Pass/Fail) for each category. You either implement the logic correctly or you don't.

*   **Public Logic Check (15%)**
    *   **Initialization:** Verifies `student_id` and `discovery_freq` are loaded correctly at startup.
    *   **Trigger:** Verifies the system transitions from `READY` → `BUSY` when the `TRIG` command is received.
    *   **Completion:** Verifies the system transitions from `BUSY` → `READY` after processing.
*   **Hidden Stress Test (15%)**
    *   **Spurious Writes:** The autograder attempts to write to other registers (e.g., `CONFIDENCE`) while the system is `BUSY`. Your logic must ignore these and remain `BUSY`.
    *   **Invalid Commands:** The autograder sends garbage commands to the Status register while `BUSY`. Your logic must ignore these.

#### **2. DoA Estimation (70 Marks)**

This section uses **Granular Scoring**. You do not need a perfect answer to get marks.
*   **Max Points per Test:** 10 points.
*   **Penalty:** -1 point per degree of error. (e.g., 3° error = 7/10 points).
*   **Failure:** >10° error results in 0 points for that test.

**Test Suite Composition:**
*   **Public Vectors (Local & Remote):**
    *   **Standard:** 0°, 30°, -45° (Clean signals).
    *   **Edge Cases:** 0° with Noise (10dB SNR), 0° Quiet (Low Amplitude).
    *   **Robustness:** NULL pointer safety check.
*   **Hidden Vectors (Remote Only):**
    *   **Full Angle Sweep:** Tests angles from **-70° to +70°** in 10° steps.
    *   **Dual Mode:** Each angle is tested twice: once with a **Clean** signal and once with **Noise** added.

---

### **✅ The "Green Tick" (Hard Gate)**
To receive the "Passing" badge on GitHub (Green Tick), you must strictly pass the **Public Tests**:
1.  **I2C:** Basic State Machine must work.
2.  **DoA:** Public angles (0, 30, -45) must be accurate to within **±2 degrees**.

*Note: You can get a Green Tick with a score of ~40% (if you fail all hidden tests), but you need the hidden tests to get an A.*

---

## � Submission Workflow

### **Milestone 1: Technical Contract**
1.  Perform physics baseline analysis in `/analysis/`.
2.  Submit the **Technical Contract PDF** to Gradescope.
3.  Ensure your partner is added to the Gradescope submission.

### **Milestone 2: Design for Manufacture**
1.  Complete PCB design in `/hardware/`.
2.  Export fabrication files to `/hardware/production/` (See the README in that folder for naming).
3.  Submit the **M2 Design Audit PDF** to Gradescope.
4.  Push final production files to this GitHub repository.

### **Milestone 3: Firmware & Integration**
1.  Pass all local tests using `./test_locally.sh` (or `.bat`).
2.  Push to GitHub and verify the **Actions** workflow passes (Green Check).
3.  Submit your final report and video demo to Gradescope.

---

## 🔍 Milestone 5: The Final Audit (Forensic Analysis)
Once your hardware is fabricated and your firmware is flashed, you will perform a final performance audit.
* **Location:** `/analysis/M5_audit.py`
* **Task:** Capture real-world data from your physical rig and compare it against the synthetic baselines generated in M3.

---

## 👤 Student / Team Information
*Fill out the section relevant to your current assignment phase.*

**For Group Phase (M1-M2):**
* **Team Number:** [e.g., Team 12]
* **Student 1:** [Name & Student Number]
* **Student 2:** [Name & Student Number]

**For Individual Phase (M3-M5):**
* **Name:** [Student Name]
* **Student Number:** [Student Number]
* **Original Team Reference:** [e.g., Team 12]
