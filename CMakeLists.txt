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

file(
    GLOB_RECURSE SOURCE_FILES
    CMAKE_CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/inc/*.hpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(${PROJECT_SOURCE_DIR}/inc)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(libtcod CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image SDL2::SDL2 SDL2::SDL2main libtcod::libtcod)