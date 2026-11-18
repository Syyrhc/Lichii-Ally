// SPDX-FileCopyrightText: 2026 Fritchii contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <array>
#include <cstdint>

namespace InputCommon::WindowsMotionMath {

struct Sample {
    std::array<float, 3> acceleration;
    std::array<float, 3> gyroscope;
};

// Polling a cached reading is not sensor progress. Keep this state across sensor
// rediscovery so reopening a stalled device cannot make its old sample fresh.
class ReadingFreshness {
public:
    constexpr bool Observe(std::int64_t timestamp, std::uint64_t now_us) {
        if (timestamp <= 0 || timestamp == last_timestamp) {
            return false;
        }
        last_timestamp = timestamp;
        last_progress_us = now_us;
        return true;
    }

    constexpr bool IsFresh(std::uint64_t now_us, std::uint64_t timeout_us) const {
        return last_timestamp > 0 && now_us >= last_progress_us &&
               now_us - last_progress_us < timeout_us;
    }

private:
    std::int64_t last_timestamp{};
    std::uint64_t last_progress_us{};
};

// WinRT reports gravity in g and angular velocity in degrees/second. Convert its
// landscape axes to the input stack's controller convention and turns/second.
constexpr Sample Convert(double accel_x, double accel_y, double accel_z, double gyro_x,
                         double gyro_y, double gyro_z) {
    return {
        .acceleration = {static_cast<float>(accel_x), static_cast<float>(-accel_z),
                         static_cast<float>(accel_y)},
        .gyroscope = {static_cast<float>(gyro_x / 360.0), static_cast<float>(-gyro_z / 360.0),
                      static_cast<float>(gyro_y / 360.0)},
    };
}

// DateTime uses 100 ns ticks. A clock reset or resume must not integrate the
// newest angular velocity over an entire suspend interval.
constexpr std::uint64_t DeltaMicroseconds(std::int64_t previous, std::int64_t current,
                                          std::uint64_t nominal) {
    if (previous <= 0 || current <= previous) {
        return nominal;
    }
    const auto ticks = static_cast<std::uint64_t>(current) - static_cast<std::uint64_t>(previous);
    const auto delta = ticks / 10;
    return delta == 0 || delta > 100'000 ? nominal : delta;
}

} // namespace InputCommon::WindowsMotionMath
