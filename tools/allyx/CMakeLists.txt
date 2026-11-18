# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.20)

# This file can also be configured independently, without downloading emulator dependencies:
# cmake -S tools/allyx -B build/motion-checks && cmake --build build/motion-checks
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(LichiiMotionChecks LANGUAGES CXX)
    enable_testing()
endif()

add_executable(allyx_motion_checks Test-MotionMath.cpp)
target_include_directories(allyx_motion_checks PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
target_compile_features(allyx_motion_checks PRIVATE cxx_std_20)
add_test(NAME allyx_motion_checks COMMAND allyx_motion_checks)

if (WIN32 AND MSVC)
    add_executable(allyx_sensor_probe Test-WindowsSensors.cpp)
    target_include_directories(allyx_sensor_probe PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
    target_compile_features(allyx_sensor_probe PRIVATE cxx_std_20)
    target_compile_options(allyx_sensor_probe PRIVATE /EHsc)
    target_link_libraries(allyx_sensor_probe PRIVATE windowsapp)
endif()
