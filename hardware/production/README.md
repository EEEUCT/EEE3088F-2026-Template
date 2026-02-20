# M2 Fabrication Files (Production Ready)

This folder contains the final output required for the EEE3088F manufacturing cycle. The course staff will use the files in this directory to place the bulk order with JLCPCB.

## 📦 Mandatory Files Checklist
Before submitting, ensure the following three files are present and named correctly:

- [ ] `TEAM_NAME_M2_v1.zip` (Gerbers & Excellon Drill files)
- [ ] `BOM.csv` (Bill of Materials with LCSC part numbers)
- [ ] `CPL.csv` (Component Placement / Centroid file)

---

## ⚠️ Critical Compliance Audit
Any violation of the following "Hard Gates" will result in your board being **REJECTED** from the fabrication batch:

### 1. Mechanical Hard-Gate
- **Mounting Pitch:** Center-to-center distance of mounting holes must be **50.0mm ± 0.2mm**.
- **Hole Diameter:** Mounting holes must be **3.2mm** (M3 clearance).
- **Collinearity:** Microphones must be aligned on the axis connecting the mounting hole centers.

### 2. Electrical Safety (The 3.3V Law)
- **Power:** The board must operate strictly on **3.3V DC**.
- **Logic:** I2C pull-up resistors (SDA/SCL) **MUST** be tied to the 3.3V rail. **Connecting pull-ups to 5V will result in immediate rejection.**
- **Current:** Total board current draw should not exceed 100mA.

### 3. Sourcing & Stock
- **LCSC Numbers:** Every component in `BOM.csv` must have a valid LCSC part number.
- **Availability:** Verify at [LCSC.com](https://www.lcsc.com) that all parts are currently in stock (>100 units).

---

## 🛠 File Generation Tips
- **Units:** All exports (Gerbers, BOM, CPL) must be in **Millimeters (mm)**.
- **Rotation:** Ensure the `Rotation` column in `CPL.csv` matches the Pin 1 orientation of your footprints.
- **Board Outline:** The board edge/outline must be included in the Gerber ZIP on a dedicated layer (Mechanical or Keep-out).



---
**Staff Note:** If these files are inconsistent with your Gradescope M2 submission, the Gradescope PDF remains the "Source of Truth" for marking, but the files in this folder are what will be built. Use at your own risk.
