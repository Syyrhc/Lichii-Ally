// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <algorithm>
#include <cmath>
#include <mutex>
#include "input_common/drivers/windows_motion_math.h"

namespace InputCommon::WindowsMotionMath {

// Callbacks own this cache, never the input engine. Idle reports retain gravity,
// but old angular velocity must not rotate the emulated controller indefinitely.
class EventSamples {
public:
    struct Frame {
        Sample sample{};
        bool valid{};
        bool fresh{};
    };

    void Acceleration(std::array<double, 3> value, std::int64_t timestamp) {
        if (!Finite(value) || timestamp <= 0)
            return;
        std::scoped_lock lock{mutex};
        acceleration = value;
        have_accel = true;
    }

    void Rotation(std::array<double, 3> value, std::int64_t timestamp, std::uint64_t now) {
        if (!Finite(value) || timestamp <= 0)
            return;
        std::scoped_lock lock{mutex};
        if (timestamp == gyro_timestamp)
            return;
        rotation = value;
        gyro_timestamp = timestamp;
        gyro_arrival = now;
    }

    Frame Read(std::uint64_t now) const {
        std::scoped_lock lock{mutex};
        Frame result;
        result.valid = have_accel && gyro_timestamp > 0;
        if (!result.valid)
            return result;
        result.fresh = now >= gyro_arrival && now - gyro_arrival < 100'000;
        const auto gyro = result.fresh ? rotation : std::array<double, 3>{};
        result.sample =
            Convert(acceleration[0], acceleration[1], acceleration[2], gyro[0], gyro[1], gyro[2]);
        return result;
    }

private:
    static bool Finite(const std::array<double, 3>& value) {
        return std::ranges::all_of(value, [](double v) { return std::isfinite(v); });
    }
    mutable std::mutex mutex;
    std::array<double, 3> acceleration{}, rotation{};
    std::int64_t gyro_timestamp{};
    std::uint64_t gyro_arrival{};
    bool have_accel{};
};
} // namespace InputCommon::WindowsMotionMath
