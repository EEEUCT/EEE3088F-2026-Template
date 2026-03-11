# /hardware/production/

This folder contains the final output required for the EEE3088F manufacturing cycle. The course staff will use the files in this directory to place the bulk order with JLCPCB.

## 📦 Mandatory Files Checklist
Before submitting, ensure the following three files are present and named correctly. **Do not use spaces in filenames.**

- [ ] `fabrication_v1.zip` (Gerbers & Excellon Drill files)
- [ ] `BOM.csv` (Bill of Materials with LCSC part numbers)
- [ ] `CPL.csv` (Component Placement / Centroid file)

#### **A. The Gerber Archive (.zip)**

This archive must contain the physical layers of your PCB.

*   **Mandatory Layers:** Top/Bottom Copper, Top/Bottom Solder Mask, Top/Bottom Silkscreen, and a **Board Outline** (on a Mechanical or Keep-out layer).
*   **Drill File:** Must be in **Excellon** format.
*   **Verification:** Use an online Gerber Viewer to ensure your mounting holes and board edges are visible. If they aren't in the viewer, they won't be on the physical board.

#### **B. The Bill of Materials (BOM.csv)**

This file tells the assembly house what to buy. It **must** include these columns:

1.  **Designator:** (e.g., C1, U2, R5)
2.  **LCSC Part Number:** (e.g., C1597). **This is the mandatory identifier.**
3.  **Quantity:** Number of units per board.
4.  **Footprint:** (e.g., 0603, SOT-23).
    
#### **C. The Component Placement File (CPL.csv)**

This file tells the robot where to place the parts.

1.  **Required Columns:** Designator, Mid X, Mid Y, Layer, Rotation.
2.  **Units:** Must be in **millimeters (mm)**.
3.  **Rotation:** You must verify that the orientation in this file matches the "Pin 1" marker on your PCB.
   
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

### 3. Sourcing & Stock
- **LCSC Numbers:** Every component in `BOM.csv` must have a valid LCSC part number.
- **Availability:** Verify at [LCSC.com](https://www.lcsc.com) that all parts are currently in stock (>100 units).

---

## 🛠 File Generation Tips
- **Units:** All exports (Gerbers, BOM, CPL) must be in **Millimeters (mm)**.
- **Rotation:** Ensure the `Rotation` column in `CPL.csv` matches the Pin 1 orientation of your footprints.
- **Board Outline:** The board edge/outline must be included in the Gerber ZIP on a dedicated layer (Mechanical or Keep-out).

---
**Staff Note:** The files in this folder are what will be built. Discrepancies between these files and your Gradescope submission may result in a manufacturing rejection.
