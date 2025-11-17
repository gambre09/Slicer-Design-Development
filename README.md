# Slicer Qt App (Starter Project)

A working Qt Widgets C++ project that maps your slicer UI design (Sections A–H) into a runnable skeleton.

## Features implemented
- Ribbon-like top toolbar with modes: **Preview, Prepare, Monitoring, FEM, Calibration**.
- **Dock widgets** for:
  - Section **B** (Tools)
  - Section **C** (Context-sensitive panel; switches with mode)
  - Section **D** (View orientations)
  - Section **G** (Print settings: Basic & Advanced)
  - Section **H** (Parameter explanations)
  - Section **F** (Toolpath simulation) — only visible in **Prepare** mode.
- Central area (Section **E**) with a placeholder **3D viewport**, printer/material selection, **Slice** and **Send to Print** buttons.

> This is a foundation you can extend with real 3D, slicing, G-code, monitoring, and FEM. All UI pieces are already wired and visible.

## Build Instructions (CMake)

### Prerequisites
- CMake 3.16+
- **Qt 6 (preferred)** or **Qt 5** with Widgets module
- C++17-capable compiler

### Configure & Build

```bash
# In project root
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
cmake --build build --config Release
# On Windows (MSVC), you can open the generated .sln or use:
cmake --build build --config Release
```

> Replace `/path/to/Qt/6.x.x/gcc_64` with your Qt install path.
> For Qt5, set `-DCMAKE_PREFIX_PATH=/path/to/Qt/5.x.x/msvc2019_64` (or equivalent).

### Run
- On Linux/macOS:
  - `./build/SlicerQtApp`
- On Windows:
  - `build\Release\SlicerQtApp.exe` (or Debug). Use `windeployqt` if needed.

## Project Structure
```
SlicerQtApp/
├── CMakeLists.txt
├── README.md
├── resources/
│   ├── icons/
│   └── resources.qrc
└── src/
    ├── CentralWidget.cpp
    ├── CentralWidget.h
    ├── MainWindow.cpp
    ├── MainWindow.h
    └── main.cpp
```
##Output:-
<img width="1099" height="703" alt="Screenshot 2025-08-26 110225" src="https://github.com/user-attachments/assets/f47b2983-471d-4379-a2ff-aaa9c7440187" />

## Next Steps
- Replace the placeholder 3D viewport with `QOpenGLWidget` or Qt3D.
- Populate **G-code preview** and **simulation**.
- Implement **mesh repair** UI and logic.
- Add **printer connection** and **monitoring**.
- FEM tab: integrate a simulation library (e.g., via Python or C++ libraries).

-Abhishek Gambre(https://www.linkedin.com/in/abhishek-gambre-566341227/)
