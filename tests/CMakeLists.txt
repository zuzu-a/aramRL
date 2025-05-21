cmake_minimum_required(VERSION 3.28.0)
project(aram_tests LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/tests") # Output test executable to a subfolder

# Include headers from the main project
target_include_directories(aram_tests PUBLIC
    "${PROJECT_SOURCE_DIR}/inc/core"
    "${PROJECT_SOURCE_DIR}/inc/game"
    # GTest headers are typically handled by target_link_libraries with GTest::GTest
)
   
# This assumes your main project isn't built as a library to link against.
# For now, we will re-compile core sources needed for tests.
# Ideally, the main project would build a library ("aram_core") and tests link against it.
# For now, let's list a few core files. This is NOT ideal and should be improved later.
# A better approach is to create a static library from core sources in the main CMakeLists.txt
# and link aram_tests against that library.

# Example: including some core sources directly (temporary solution)
# file(GLOB CORE_TEST_SOURCES
#     "${PROJECT_SOURCE_DIR}/src/core/EntityManager.cpp"
#     "${PROJECT_SOURCE_DIR}/src/core/tileset.cpp"
# )

file(GLOB TEST_SOURCES "*.cpp" "core/*.cpp") # Or list explicitly

add_executable(aram_tests ${TEST_SOURCES}) # ${CORE_TEST_SOURCES}
   
# Link GTest
# Ensure GTest was found in the root CMakeLists.txt
target_link_libraries(aram_tests PRIVATE GTest::GTest GTest::Main SDL2::SDL2 SDL2_image::SDL2_image)

# Enable testing and Google Test integration
include(CTest)
include(GoogleTest)
gtest_discover_tests(aram_tests)
