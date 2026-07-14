# C++ / OpenGL Flight Simulator

A real-time 3D flight simulation engine written in modern C++ utilizing hardware-accelerated OpenGL for rendering, STL file parsing for loading 3D airplane models, and a custom text rendering engine.

## Features

- **Real-time 3D Flight Physics**: Implements rotational mechanics across Pitch, Roll, and Yaw axes based on localized aircraft direction vectors.
- **Dynamic 3D Model Loading**: High-performance binary STL model parser that dynamically constructs vertex and normal buffer structures.
- **Multi-Camera Systems**: Integrated camera matrix configurations featuring cockpit view, follow view, and cinematic perspectives.
- **Custom GUI & Bitmap Typography**: Low-overhead 2D typography layer utilizing custom 6x8 bitmap font lookups paired with a dynamically grouped vertex buffer layout.
- **Integrated Autopilot Subsystem**: Rule-based autopilot system featuring altitude climbs, navigational target tracking, and automated alignment stages.
- **Procedural Terrain & Obstacles**: Infinite ground tile repetition using mipmapped texture wrapping paired with structural layout distribution structures (water bodies, structures, vegetation arrays).
- **Modular Crash Physics**: Multi-element structural disintegration and rigid fragment distribution vectors tracking grounded collision states upon impact terminal events.

---

## Directory Structure

```
├── models/                  # Binary .stl 3D aircraft models (e.g., A350, 747)
├── textures/                # Asset textures (ground, skybox, elements)
├── src/
│   ├── main.cpp             # Simulation entry point
│   ├── Simulation.h/.cpp    # Physics loop and main simulation manager
│   ├── Sim_Setup.cpp        # 3D assets loading and simulator initialization
│   ├── Shaders.h/.cpp       # Core OpenGL shader sources and compiled shader programs
│   ├── HUD.cpp              # UI logic and cockpit HUD interface management
│   ├── graphic_tools_2d.cpp # Dedicated 2D batch renderer and UI primitives
│   ├── game_config.h/.cpp   # Static environment configuration & aircraft context metadata
│   └── bitmap_font.h        # Raw bitmap font definitions for the custom UI
└── README.md                # Project documentation
```

## Prerequisites & Environment Layout

### System & Core Prerequisites
* **Graphics Driver Environment:** Hardware acceleration backing an **OpenGL 3.3+** Core Profile context. Programmable vertex and fragment shader processing pipelines are mandatory.
* **Toolchain Requirements:** A modern **C++ compiler** supporting **C++11 or newer** standards to handle fixed architecture optimizations and explicit heap instantiation lifetimes.
* **Display Server Hooks:** Standard system-level base packages targeting window management sub-allocations and native window graphics context bindings.

### Core Ecosystem Libraries
The application links explicitly against three foundational graphics development libraries:
* **GLEW (The OpenGL Extension Wrangler Library):** Provides runtime querying mechanisms and loads target function pointers for advanced hardware-level graphics extensions.
* **GLFW3 (Graphics Library Framework):** An abstraction layer handling native window configuration lifecycle boundaries, hardware context mapping, and low-latency keyboard/mouse input states.
* **GLM (OpenGL Mathematics):** A header-only, performance-optimized vector, matrix, and quaternion mathematics system tailored directly to GLSL design architectures.

### Asset & System Dependencies
Internal subsystems rely on a blend of native engine headers and structural toolkit dependencies:
* **`stb_image.h`:** A light, high-performance single-header image utility compiled into the data pipeline to extract bit-matrix dimensions from skybox maps and landscape asset targets.
* **Standard Template Library (STL) Utilities:** The underlying state synchronization structures use the platform toolchain's foundational headers directly:
  - `iostream` & `fstream`: High-frequency linear stream controllers facilitating file-system input loops and binary `.stl` flight model reading.
  - `vector`: Flexible heap memory blocks used heavily to queue 3D geometric arrays and optimize dynamic 2D GUI vertex buffering pipelines.
  - `cmath` & `algorithm`: Floating-point processing hooks used to calculate cross-product pitch vectors, yaw alignments, roll bounds, and vector ranges.
  - `chrono`, `thread`, & `random`: Thread-safe time frame synchronization layers used to calculate steady FPS step ticks and run randomized macro distribution seeds.

## Dependency Installation

### 1. Ubuntu / Debian Linux
Open your terminal and run the following commands to install the required development packages and compiler utilities via `apt`:

```bash
# Update system package index registries
sudo apt-get update

# Install general compilation tools and building blocks
sudo apt-get install build-essential

# Install development headers for GLEW, GLFW3, and GLM
sudo apt-get install libglew-dev libglfw3-dev libglm-dev
```

## Compilation: 
- Makefile route:
    - make
    - ./flight-sim
- g++ command:
    - g++ main.cpp graphic_tools_2d.cpp Simulation.cpp Shaders.cpp game_config.cpp Sim_Setup.cpp HUD.cpp -o flight-sim -lGLEW -lglfw -lGL
    - ./flight-sim

## Known limitations:
- Rigid body collisions treat complex geometry approximations as uniform grid coordinate bounds.
- Binary STL input parser is strictly bound to standardized 50-byte header formats without individual facet attribute error handling.
- Environment is limited to two localized airports.
- Collision tracking is bound to terrain height levels; aircraft can pass through objects other than land geometry.
- Landing Gear and flaps are currently not implemented.
- Procedurally distributed structures can occasionally overlap directly onto active runway pathways.
- six level inheritance
- no shader compile checks





