# EEE3088F: DoA Project Repository (Group Phase)

This repository hosts the design, analysis, and fabrication data for the **Acoustic Direction of Arrival (DoA)** project.

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

### 3. Submission & Autograding (Hidden Tests)
When you push your code to GitHub, the **Actions** tab runs the "Firmware Autograder". This runs the public tests **PLUS** a set of **Hidden/Secret** tests:
*   **Edge Cases:** Angles near 0° (Boresight) and 90° (End-fire).
*   **Stress Tests:** High noise scenarios and randomized angles to prevent hardcoding.
*   **Pass Criteria:** You must pass ALL tests (Public + Hidden) to receive the green checkmark.

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

## 👥 Team
* **Student 1:** [Name/Student Number]
* **Student 2:** [Name/Student Number]
