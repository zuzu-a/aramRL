cmake_minimum_required(VERSION 3.28.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(
    aram
    VERSION 0.1.0
    LANGUAGES C CXX
    )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib") # For static libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib") # For shared libraries

# Core engine source files
set(CORE_SOURCES
    src/core/Engine.cpp
    src/core/render.cpp
    src/core/rogue.cpp
    src/core/sdl/EventHandler.cpp
    src/core/systems/InputSystem.cpp # Already present, but good to confirm
    src/core/systems/MovementSystem.cpp
    # src/core/systems/PositionSystem.cpp # Removed
    src/core/systems/TileSystem.cpp
    # src/core/tiles.cpp # Removed
    src/core/tileset.cpp # Should be present from Tileset class creation
    src/core/Tilemap.cpp # Added Tilemap source
)

# Core engine header files (listed for clarity, not typically used in set() for sources)
set(CORE_HEADERS
    inc/core/Engine.hpp
    inc/core/Tileset.hpp # Relocated
    inc/core/Tilemap.hpp # Added Tilemap header
    inc/core/components/player.hpp
    # inc/core/components/tileset.hpp # Removed old path
    inc/core/components/transform.hpp
    inc/core/components/vector2d.hpp
    inc/core/render.hpp
    inc/core/rogue.hpp
    inc/core/sdl/EventHandler.hpp
    inc/core/components/InputComponent.hpp # Added InputComponent header
    inc/core/systems/InputSystem.hpp # Already present, but good to confirm
    inc/core/systems/MovementSystem.hpp # Added MovementSystem header
    # inc/core/systems/PositionSystem.hpp # Removed
    inc/core/systems/TileSystem.hpp # Added TileSystem header
    inc/core/systems.hpp
    # inc/core/tileset.hpp # This was the old Tileset header, now inc/core/Tileset.hpp
    inc/core/tools/errors.hpp
)

# Game specific source files
set(GAME_SOURCES
    src/game/main.cpp
)

# Game specific header files (empty for now)
# set(GAME_HEADERS
# )

add_executable(${PROJECT_NAME} ${CORE_SOURCES} ${GAME_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/inc/core"
    "${PROJECT_SOURCE_DIR}/inc/game"
    # Vcpkg should handle its include directories automatically for linked libraries
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Build Configurations
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

# Debug flags
if(CMAKE_BUILD_TYPE MATCHES "^[Dd][Ee][Bb][Uu][Gg]$")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0) # Example for GCC/Clang
    # target_compile_definitions(${PROJECT_NAME} PRIVATE ARAM_DEBUG)
endif()

# Release flags
if(CMAKE_BUILD_TYPE MATCHES "^[Rr][Ee][Ll][Ee][Aa][Ss][Ee]$")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -DNDEBUG) # Example for GCC/Clang
    # target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest REQUIRED) # For tests

# entt is header-only but vcpkg might provide an imported target.
# If entt::entt is not found, just ensuring includes are correct is enough.
# For now, assume entt::entt is a valid target from vcpkg.
target_link_libraries(${PROJECT_NAME} PRIVATE
    entt::entt
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    nlohmann_json::nlohmann_json
)

add_subdirectory(tests)