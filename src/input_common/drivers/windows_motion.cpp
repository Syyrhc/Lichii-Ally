// SPDX-FileCopyrightText: 2026 Fritchii contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <memory>

#include <winrt/Windows.Devices.Sensors.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Display.h>

#include "common/logging.h"
#include "common/scope_exit.h"
#include "common/settings_input.h"
#include "common/thread.h"
#include "input_common/drivers/windows_motion.h"
#include "input_common/drivers/windows_motion_events.h"

namespace InputCommon {
namespace {

constexpr auto RetryInterval = std::chrono::seconds{3};
constexpr u32 RequestedIntervalMs = 10;

u64 MotionClockUs() {
    return static_cast<u64>(std::chrono::duration_cast<std::chrono::microseconds>(
                                std::chrono::steady_clock::now().time_since_epoch())
                                .count());
}
} // namespace

WindowsMotion::WindowsMotion(std::string input_engine_) : InputEngine(std::move(input_engine_)) {
    PreSetController(identifier);
    PreSetMotion(identifier, 0);
    worker = std::jthread{[this](std::stop_token stop_token) { Run(stop_token); }};
}

WindowsMotion::~WindowsMotion() {
    worker.request_stop();
    wait_condition.notify_all();
    if (worker.joinable()) {
        worker.join();
    }
}

bool WindowsMotion::IsAvailable() const {
    return available.load(std::memory_order_relaxed);
}

std::vector<Common::ParamPackage> WindowsMotion::GetInputDevices() const {
    if (!IsAvailable()) {
        return {};
    }
    Common::ParamPackage params;
    params.Set("engine", GetEngineName());
    params.Set("display", "Built-in Windows motion (ROG Ally X)");
    params.Set("guid", identifier.guid.RawString());
    params.Set("port", 0);
    params.Set("pad", 0);
    params.Set("motion_only", true);
    return {params};
}

MotionMapping WindowsMotion::GetMotionMappingForDevice(const Common::ParamPackage& params) {
    if (params.Get("engine", "") != GetEngineName()) {
        return {};
    }
    Common::ParamPackage motion;
    motion.Set("engine", GetEngineName());
    motion.Set("guid", identifier.guid.RawString());
    motion.Set("port", 0);
    motion.Set("pad", 0);
    motion.Set("motion", 0);
    return {{Settings::NativeMotion::MotionLeft, motion},
            {Settings::NativeMotion::MotionRight, motion}};
}

Common::Input::ButtonNames WindowsMotion::GetUIName(const Common::ParamPackage& params) const {
    return params.Has("motion") ? Common::Input::ButtonNames::Engine
                                : Common::Input::ButtonNames::Invalid;
}

bool WindowsMotion::WaitUntil(std::stop_token stop_token,
                              std::chrono::steady_clock::time_point deadline) {
    std::unique_lock lock{wait_mutex};
    wait_condition.wait_until(lock, stop_token, deadline, [] { return false; });
    return !stop_token.stop_requested();
}

void WindowsMotion::ClearMotion() {
    available.store(false, std::memory_order_relaxed);
    SetMotion(identifier, 0, BasicMotion{.delta_timestamp = 0});
}

void WindowsMotion::Run(std::stop_token stop_token) {
    using Clock = std::chrono::steady_clock;
    using winrt::Windows::Devices::Sensors::Accelerometer;
    using winrt::Windows::Devices::Sensors::AccelerometerDataThreshold;
    using winrt::Windows::Devices::Sensors::Gyrometer;
    using winrt::Windows::Devices::Sensors::GyrometerDataThreshold;
    using winrt::Windows::Graphics::Display::DisplayOrientations;

    Common::SetCurrentThreadName("NativeMotion");
    try {
        winrt::init_apartment(winrt::apartment_type::multi_threaded);
    } catch (const winrt::hresult_error& error) {
        LOG_ERROR(Input, "Could not initialize native Windows motion (HRESULT 0x{:08X})",
                  static_cast<u32>(error.code().value));
        return;
    }
    SCOPE_EXIT {
        winrt::uninit_apartment();
    };

    bool missing_logged = false;

    bool failure_logged = false;

    while (!stop_token.stop_requested()) {
        try {
            auto gyroscope = Gyrometer::GetDefault();
            auto accelerometer = Accelerometer::GetDefault();
            if (gyroscope && accelerometer) {
                const auto previous_gyro_interval = gyroscope.ReportInterval();
                const auto previous_accel_interval = accelerometer.ReportInterval();
                const auto previous_gyro_transform = gyroscope.ReadingTransform();
                const auto previous_accel_transform = accelerometer.ReadingTransform();
                GyrometerDataThreshold gyro_threshold{nullptr};
                AccelerometerDataThreshold accel_threshold{nullptr};
                std::array<double, 3> previous_gyro_threshold{};
                std::array<double, 3> previous_accel_threshold{};
                bool gyro_threshold_saved = false;
                bool accel_threshold_saved = false;
                SCOPE_EXIT {
                    ClearMotion();
                    try {
                        gyroscope.ReportInterval(previous_gyro_interval);
                    } catch (const winrt::hresult_error&) {
                    }
                    try {
                        accelerometer.ReportInterval(previous_accel_interval);
                    } catch (const winrt::hresult_error&) {
                    }
                    try {
                        gyroscope.ReadingTransform(previous_gyro_transform);
                    } catch (const winrt::hresult_error&) {
                    }
                    try {
                        accelerometer.ReadingTransform(previous_accel_transform);
                    } catch (const winrt::hresult_error&) {
                    }
                    if (gyro_threshold_saved) {
                        try {
                            gyro_threshold.XAxisInDegreesPerSecond(previous_gyro_threshold[0]);
                            gyro_threshold.YAxisInDegreesPerSecond(previous_gyro_threshold[1]);
                            gyro_threshold.ZAxisInDegreesPerSecond(previous_gyro_threshold[2]);
                        } catch (const winrt::hresult_error&) {
                        }
                    }
                    if (accel_threshold_saved) {
                        try {
                            accel_threshold.XAxisInGForce(previous_accel_threshold[0]);
                            accel_threshold.YAxisInGForce(previous_accel_threshold[1]);
                            accel_threshold.ZAxisInGForce(previous_accel_threshold[2]);
                        } catch (const winrt::hresult_error&) {
                        }
                    }
                };

                try {
                    gyro_threshold = gyroscope.ReportThreshold();
                    if (gyro_threshold) {
                        previous_gyro_threshold = {gyro_threshold.XAxisInDegreesPerSecond(),
                                                   gyro_threshold.YAxisInDegreesPerSecond(),
                                                   gyro_threshold.ZAxisInDegreesPerSecond()};
                        gyro_threshold_saved = true;
                    }
                } catch (const winrt::hresult_error&) {
                }
                try {
                    accel_threshold = accelerometer.ReportThreshold();
                    if (accel_threshold) {
                        previous_accel_threshold = {accel_threshold.XAxisInGForce(),
                                                    accel_threshold.YAxisInGForce(),
                                                    accel_threshold.ZAxisInGForce()};
                        accel_threshold_saved = true;
                    }
                } catch (const winrt::hresult_error&) {
                }

                const auto interval_ms =
                    std::max({RequestedIntervalMs, gyroscope.MinimumReportInterval(),
                              accelerometer.MinimumReportInterval()});
                const auto interval = std::chrono::milliseconds{interval_ms};

                gyroscope.ReadingTransform(DisplayOrientations::Landscape);
                accelerometer.ReadingTransform(DisplayOrientations::Landscape);
                gyroscope.ReportInterval(interval_ms);
                accelerometer.ReportInterval(interval_ms);

                if (gyro_threshold_saved) {
                    try {
                        gyro_threshold.XAxisInDegreesPerSecond(0.0);
                        gyro_threshold.YAxisInDegreesPerSecond(0.0);
                        gyro_threshold.ZAxisInDegreesPerSecond(0.0);
                    } catch (const winrt::hresult_error&) {
                    }
                }
                if (accel_threshold_saved) {
                    try {
                        accel_threshold.XAxisInGForce(0.0);
                        accel_threshold.YAxisInGForce(0.0);
                        accel_threshold.ZAxisInGForce(0.0);
                    } catch (const winrt::hresult_error&) {
                    }
                }

                auto samples = std::make_shared<WindowsMotionMath::EventSamples>();
                // Revoke subscriptions before restoring settings. In-flight callbacks
                // retain only the cache, so destruction cannot access the input engine.
                auto accel_events = accelerometer.ReadingChanged(
                    winrt::auto_revoke, [samples](const auto&, const auto& args) {
                        try {
                            const auto reading = args.Reading();
                            samples->Acceleration({reading.AccelerationX(), reading.AccelerationY(),
                                                   reading.AccelerationZ()},
                                                  reading.Timestamp().time_since_epoch().count());
                        } catch (const winrt::hresult_error&) {
                        }
                    });
                auto gyro_events = gyroscope.ReadingChanged(
                    winrt::auto_revoke, [samples](const auto&, const auto& args) {
                        try {
                            const auto reading = args.Reading();
                            samples->Rotation(
                                {reading.AngularVelocityX(), reading.AngularVelocityY(),
                                 reading.AngularVelocityZ()},
                                reading.Timestamp().time_since_epoch().count(), MotionClockUs());
                        } catch (const winrt::hresult_error&) {
                        }
                    });
                const auto session_start = Clock::now();
                auto last_emission = session_start - interval;
                auto next_poll = session_start;
                while (!stop_token.stop_requested()) {
                    const auto now = Clock::now();
                    const auto frame = samples->Read(MotionClockUs());
                    if (frame.valid) {
                        if (!available.exchange(true, std::memory_order_relaxed)) {
                            LOG_INFO(Input,
                                     "Native Windows motion ready: event delivery, {} ms interval "
                                     "(motion fix 2)",
                                     interval_ms);
                            missing_logged = false;
                            failure_logged = false;
                        }
                        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                                                 now - last_emission)
                                                 .count();
                        const auto delta = elapsed > 0 && elapsed <= 100'000
                                               ? static_cast<u64>(elapsed)
                                               : static_cast<u64>(interval_ms) * 1000;
                        last_emission = now;
                        const auto& sample = frame.sample;
                        SetMotion(identifier, 0,
                                  BasicMotion{.gyro_x = sample.gyroscope[0],
                                              .gyro_y = sample.gyroscope[1],
                                              .gyro_z = sample.gyroscope[2],
                                              .accel_x = sample.acceleration[0],
                                              .accel_y = sample.acceleration[1],
                                              .accel_z = sample.acceleration[2],
                                              .delta_timestamp = delta});
                    } else if (now - session_start > std::chrono::seconds{30}) {
                        LOG_WARNING(
                            Input,
                            "Native Windows motion received no initial sensor pair; retrying");
                        break;
                    }
                    // Reporting gaps at rest do not tear down a working subscription.
                    next_poll += interval;
                    if (next_poll < Clock::now())
                        next_poll = Clock::now() + interval;
                    if (!WaitUntil(stop_token, next_poll))
                        break;
                }
            } else if (!missing_logged) {
                LOG_INFO(Input, "Native Windows motion unavailable: gyroscope={}, accelerometer={}",
                         static_cast<bool>(gyroscope), static_cast<bool>(accelerometer));
                missing_logged = true;
            }
        } catch (const winrt::hresult_error& error) {
            if (!failure_logged) {
                LOG_WARNING(Input,
                            "Native Windows motion failed (HRESULT 0x{:08X}); retrying sensors",
                            static_cast<u32>(error.code().value));
                failure_logged = true;
            }
        } catch (const std::exception& error) {
            if (!failure_logged) {
                LOG_WARNING(Input, "Native Windows motion failed: {}; retrying sensors",
                            error.what());
                failure_logged = true;
            }
        }
        ClearMotion();
        if (!WaitUntil(stop_token, Clock::now() + RetryInterval)) {
            break;
        }
    }
}

} // namespace InputCommon
