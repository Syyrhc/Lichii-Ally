# SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

# SPDX-FileCopyrightText: 2018 yuzu Emulator Project
# SPDX-License-Identifier: GPL-2.0-or-later

add_library(input_common STATIC
    drivers/camera.cpp
    drivers/camera.h
    drivers/keyboard.cpp
    drivers/keyboard.h
    drivers/mouse.cpp
    drivers/mouse.h
    drivers/tas_input.cpp
    drivers/tas_input.h
    drivers/touch_screen.cpp
    drivers/touch_screen.h
    drivers/udp_client.cpp
    drivers/udp_client.h
    drivers/virtual_amiibo.cpp
    drivers/virtual_amiibo.h
    drivers/virtual_gamepad.cpp
    drivers/virtual_gamepad.h
    helpers/stick_from_buttons.cpp
    helpers/stick_from_buttons.h
    helpers/touch_from_buttons.cpp
    helpers/touch_from_buttons.h
    helpers/udp_protocol.cpp
    helpers/udp_protocol.h
    input_engine.cpp
    input_engine.h
    input_mapping.cpp
    input_mapping.h
    input_poller.cpp
    input_poller.h
    main.cpp
    main.h
)

set(WINDOWS_MOTION_DEFAULT OFF)
if (WIN32 AND MSVC)
    set(WINDOWS_MOTION_DEFAULT ON)
endif()
option(ENABLE_WINDOWS_MOTION "Read built-in Windows gyroscope and accelerometer sensors" ${WINDOWS_MOTION_DEFAULT})

if (ENABLE_WINDOWS_MOTION)
    if (NOT WIN32 OR NOT MSVC)
        message(FATAL_ERROR "Native Windows motion requires MSVC or clang-cl and the Windows SDK. Use -DENABLE_WINDOWS_MOTION=OFF for other toolchains.")
    endif()
    include(CMakePushCheckState)
    include(CheckCXXSourceCompiles)
    cmake_push_check_state(RESET)
    set(CMAKE_REQUIRED_LIBRARIES windowsapp)
    check_cxx_source_compiles("
        #include <winrt/Windows.Foundation.h>
        #include <winrt/Windows.Devices.Sensors.h>
        #include <winrt/Windows.Graphics.Display.h>
        int main() {
            winrt::init_apartment(winrt::apartment_type::multi_threaded);
            auto gyro = winrt::Windows::Devices::Sensors::Gyrometer::GetDefault();
            auto accel = winrt::Windows::Devices::Sensors::Accelerometer::GetDefault();
            if (gyro && accel) {
                gyro.ReadingTransform(winrt::Windows::Graphics::Display::DisplayOrientations::Landscape);
                gyro.ReportThreshold().XAxisInDegreesPerSecond(0.0);
                accel.ReportThreshold().XAxisInGForce(0.0);
            }
            return 0;
        }" HAVE_WINDOWS_SENSORS_API)
    cmake_pop_check_state()
    if (NOT HAVE_WINDOWS_SENSORS_API)
        message(FATAL_ERROR "Native Windows motion requires a Windows SDK version 10.0.19041 or newer with C++/WinRT headers and WindowsApp.lib. Install it with Visual Studio, then configure from its x64 developer environment. See CMakeConfigureLog.yaml for compiler details.")
    endif()
    target_sources(input_common PRIVATE
        drivers/windows_motion.cpp
        drivers/windows_motion.h
        drivers/windows_motion_math.h)
    target_compile_definitions(input_common PUBLIC HAVE_WINDOWS_MOTION)
    target_link_libraries(input_common PRIVATE windowsapp)
endif()

if (MSVC)
    target_compile_options(input_common PRIVATE
        /we4242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
        /we4254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
        /we4800 # Implicit conversion from 'type' to bool. Possible information loss
    )
else()
    target_compile_options(input_common PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-Wconversion>)
endif()

if (ANDROID)
    target_sources(input_common PRIVATE
        drivers/android.cpp
        drivers/android.h)

    target_link_libraries(input_common PRIVATE android)
else()
    target_sources(input_common PRIVATE
        drivers/joycon.cpp
        drivers/joycon.h
        drivers/sdl_driver.cpp
        drivers/sdl_driver.h
        helpers/joycon_driver.cpp
        helpers/joycon_driver.h
        helpers/joycon_protocol/calibration.cpp
        helpers/joycon_protocol/calibration.h
        helpers/joycon_protocol/common_protocol.cpp
        helpers/joycon_protocol/common_protocol.h
        helpers/joycon_protocol/generic_functions.cpp
        helpers/joycon_protocol/generic_functions.h
        helpers/joycon_protocol/joycon_types.h
        helpers/joycon_protocol/irs.cpp
        helpers/joycon_protocol/irs.h
        helpers/joycon_protocol/nfc.cpp
        helpers/joycon_protocol/nfc.h
        helpers/joycon_protocol/poller.cpp
        helpers/joycon_protocol/poller.h
        helpers/joycon_protocol/ringcon.cpp
        helpers/joycon_protocol/ringcon.h
        helpers/joycon_protocol/rumble.cpp
        helpers/joycon_protocol/rumble.h)

    target_link_libraries(input_common PRIVATE SDL3::SDL3)
    target_compile_definitions(input_common PRIVATE HAVE_SDL3)
endif()

if (ENABLE_LIBUSB)
    target_sources(input_common PRIVATE
        drivers/gc_adapter.cpp
        drivers/gc_adapter.h)

    target_link_libraries(input_common PRIVATE libusb::usb)
    target_compile_definitions(input_common PRIVATE ENABLE_LIBUSB)
endif()

create_target_directory_groups(input_common)
target_link_libraries(input_common PUBLIC hid_core PRIVATE common Boost::headers)
