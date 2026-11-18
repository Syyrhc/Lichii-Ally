// SPDX-License-Identifier: GPL-3.0-or-later

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <winrt/Windows.Devices.Sensors.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Display.h>
#include "input_common/drivers/windows_motion_math.h"

using namespace winrt::Windows::Devices::Sensors;
using winrt::Windows::Graphics::Display::DisplayOrientations;

// Each client owns its transform; always restore the requested report intervals
// too.
struct SensorSettings {
    Gyrometer gyro;
    Accelerometer accel;
    uint32_t gyro_interval;
    uint32_t accel_interval;
    DisplayOrientations gyro_transform;
    DisplayOrientations accel_transform;

    ~SensorSettings() {
        try {
            gyro.ReportInterval(gyro_interval);
        } catch (const winrt::hresult_error&) {
        }
        try {
            accel.ReportInterval(accel_interval);
        } catch (const winrt::hresult_error&) {
        }
        try {
            gyro.ReadingTransform(gyro_transform);
        } catch (const winrt::hresult_error&) {
        }
        try {
            accel.ReadingTransform(accel_transform);
        } catch (const winrt::hresult_error&) {
        }
    }
};

int main() {
    try {
        winrt::init_apartment(winrt::apartment_type::multi_threaded);
        auto gyro = Gyrometer::GetDefault();
        auto accel = Accelerometer::GetDefault();
        if (!gyro || !accel) {
            std::cerr << "Native Windows gyro/accelerometer unavailable.\n";
            return 2;
        }
        SensorSettings restore{gyro,
                               accel,
                               gyro.ReportInterval(),
                               accel.ReportInterval(),
                               gyro.ReadingTransform(),
                               accel.ReadingTransform()};
        const auto interval =
            std::max({10U, gyro.MinimumReportInterval(), accel.MinimumReportInterval()});
        gyro.ReadingTransform(DisplayOrientations::Landscape);
        accel.ReadingTransform(DisplayOrientations::Landscape);
        gyro.ReportInterval(interval);
        accel.ReportInterval(interval);
        int gyro_updates = 0;
        int accel_updates = 0;
        int finite_samples = 0;
        int64_t previous_gyro = 0;
        int64_t previous_accel = 0;
        for (int i = 0; i < 100; ++i) {
            auto g = gyro.GetCurrentReading();
            auto a = accel.GetCurrentReading();
            if (g && a) {
                const auto gt = g.Timestamp().time_since_epoch().count();
                const auto at = a.Timestamp().time_since_epoch().count();
                if (gt > 0 && gt != previous_gyro)
                    ++gyro_updates;
                if (at > 0 && at != previous_accel)
                    ++accel_updates;
                previous_gyro = gt;
                previous_accel = at;
                const auto sample = InputCommon::WindowsMotionMath::Convert(
                    a.AccelerationX(), a.AccelerationY(), a.AccelerationZ(), g.AngularVelocityX(),
                    g.AngularVelocityY(), g.AngularVelocityZ());
                if (std::ranges::all_of(sample.acceleration,
                                        [](float v) { return std::isfinite(v); }) &&
                    std::ranges::all_of(sample.gyroscope,
                                        [](float v) { return std::isfinite(v); })) {
                    ++finite_samples;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{interval});
        }
        std::cout << "WinRT sensors: interval=" << interval << " ms; gyro updates=" << gyro_updates
                  << "; accelerometer updates=" << accel_updates
                  << "; finite samples=" << finite_samples << "/100\n";
        return gyro_updates > 1 && accel_updates > 1 && finite_samples == 100 ? 0 : 3;
    } catch (const winrt::hresult_error& error) {
        std::cerr << "Windows sensor API failed: HRESULT 0x" << std::hex
                  << static_cast<uint32_t>(error.code().value) << '\n';
        return 1;
    }
}
