# Aram Game Engine

## Overview

Aram is a 2D game engine currently under development using C++, SDL2, and the Entt library. It aims to serve as a foundational baseplate for game development, particularly for projects like game jams or as a learning tool for engine architecture.

**Current Status:** The foundational elements of the engine are in place, including a basic rendering pipeline, an Entity-Component-System (ECS) architecture via Entt, input handling, and some core systems like movement and resource management. The project is actively being refactored and expanded.

## Core Features

*   **Rendering:** SDL2-based 2D renderer capable of drawing primitives and placeholder characters.
*   **ECS:** Utilizes the [Entt](https://github.com/skypjack/entt) library for a high-performance Entity-Component-System architecture.
*   **Input:** Basic keyboard input handling for player actions (e.g., attack) and movement (WASD).
*   **Movement:** A simple movement system updates entity positions based on input.
*   **Asset Management:** A `ResourceManager` is implemented for managing textures and tilesets. Further development is planned for more comprehensive asset support.
*   **Tile System:** Basic `Tileset` and `Tilemap` classes are defined, with a stubbed `TileSystem` for rendering tilemaps (currently draws a debug grid).
*   **Build System:** Uses CMake (version 3.28.0 or higher recommended) for building the project, with [vcpkg](https://github.com/microsoft/vcpkg) integrated for C++ library dependency management.
*   **Testing:** The [Google Test](https://github.com/google/googletest) framework is used for unit testing core components and systems.
*   **CI (Continuous Integration):** GitHub Actions are set up for automated builds and tests on push/pull_request to the main branch, covering Debug and Release configurations.

## Folder Structure

*   `inc/core/`: Header files for the core engine components and systems.
*   `inc/game/`: Header files specific to any game built on top of the engine (currently minimal).
*   `src/core/`: Source files for the core engine components and systems.
*   `src/game/`: Source files specific to any game built on top of the engine (e.g., `main.cpp`).
*   `assets/`: Intended for game assets like textures, fonts, audio files, and data files (e.g., JSON for tilesets).
*   `docs/`: Project documentation (currently a placeholder).
*   `tests/`: Unit tests for the engine, using Google Test. Includes test data in `tests/data/`.
*   `vcpkg/`: Git submodule for the vcpkg C++ library manager.
*   `.github/workflows/`: Configuration files for GitHub Actions CI workflows.

## Prerequisites

*   **C++ Compiler:** A modern C++ compiler supporting C++23.
*   **CMake:** Version 3.20 or higher (ideally 3.28.0 or newer, matching the project's `cmake_minimum_required`).
*   **Git:** Required for cloning the repository and managing the vcpkg submodule.

## Building the Project

1.  **Clone the repository:**
    Make sure to clone recursively to include the vcpkg submodule.
    ```bash
    git clone <your_repository_url_here> --recursive
    cd aram-engine # Or your repository's directory name
    ```

2.  **Vcpkg Setup (Automatic with CMake):**
    The project is configured to use vcpkg via a CMake toolchain file. Vcpkg will automatically download and build the required dependencies when CMake configures the project. If you encounter issues, you might need to manually bootstrap vcpkg:
    ```bash
    ./vcpkg/bootstrap-vcpkg.sh  # For Linux/macOS
    # .\vcpkg\bootstrap-vcpkg.bat # For Windows
    ```

3.  **CMake Configuration:**
    Create a build directory and configure CMake.
    ```bash
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
    ```
    You can specify the build type for single-configuration generators (like Makefiles or Ninja):
    ```bash
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug
    # Or -DCMAKE_BUILD_TYPE=Release
    ```
    For multi-configuration generators (like Visual Studio), you'll select the build type when building.

4.  **Compile:**
    Build the project using CMake's build command:
    ```bash
    cmake --build build --config Debug   # For Debug build
    # cmake --build build --config Release # For Release build
    ```
    If using Makefiles or Ninja, you can also run `make` or `ninja` directly in the `build` directory after configuration.

## Running Tests

Tests are built by default along with the main application. The test executable is named `aram_tests`.

1.  **From the `build` directory, run CTest:**
    ```bash
    cd build
    ctest -C Debug # Or Release, depending on your build configuration
    # For more verbose output:
    # ctest -C Debug --output-on-failure
    ```

2.  **Alternatively, run the executable directly:**
    The test executable is typically found at `build/bin/tests/aram_tests`.
    ```bash
    ./build/bin/tests/aram_tests
    ```

## Running the Application

The main application executable is named `aram`.

1.  **Run from the build directory or directly:**
    The executable is typically found at `build/bin/aram`.
    ```bash
    ./build/bin/aram
    ```
    On Windows, the path might include the configuration, e.g., `build\bin\Debug\aram.exe`.

## Dependencies

The project uses the following main third-party libraries, managed by vcpkg:

*   **SDL2:** For windowing, input, and rendering.
*   **SDL2_image:** For loading various image formats (e.g., PNG).
*   **Entt:** For the Entity-Component-System architecture.
*   **nlohmann/json:** For parsing JSON files (e.g., for tileset data).
*   **Google Test:** For unit testing.

## Future Work / TODOs

This engine is a work in progress. Key areas for future development include:

*   **Rendering Enhancements:**
    *   Complete `TileSystem` to render actual tilemaps using loaded tilesets and tilemaps.
    *   Implement sprite rendering and animation.
    *   Add camera system (scrolling, zoom).
    *   Basic text rendering.
*   **Asset Management:**
    *   Full implementation for loading and managing fonts, audio, and other asset types.
    *   Hot reloading of assets.
*   **Game Systems:**
    *   UI System.
    *   Audio System.
    *   Basic Physics/Collision Detection.
*   **Gameplay Logic:**
    *   Develop actual game mechanics and states.
*   **Editor/Tools:**
    *   Consider simple editor tools for scene or map creation.
*   **Platform Support:**
    *   Ensure and test cross-platform compatibility (Windows, Linux, macOS).

Contributions and suggestions are welcome!
