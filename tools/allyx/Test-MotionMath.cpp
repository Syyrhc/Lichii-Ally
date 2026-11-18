// SPDX-FileCopyrightText: 2026 Fritchii contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>

#include "input_common/drivers/windows_motion_events.h"

using namespace InputCommon::WindowsMotionMath;

void Check(bool passed, int line) {
    if (!passed) {
        std::cerr << "Motion check failed at line " << line << '\n';
        std::exit(EXIT_FAILURE);
    }
}

#define CHECK(condition) Check((condition), __LINE__)

int main() {
    EventSamples events;
    CHECK(!events.Read(0).valid);
    events.Acceleration({0, 0, -1}, 1);
    events.Rotation({360, 0, 0}, 1, 1000);
    CHECK(events.Read(1000).valid && events.Read(1000).fresh);
    CHECK(events.Read(1000).sample.gyroscope[0] == 1.0f);
    events.Rotation({720, 0, 0}, 1, 90000); // Duplicate must not refresh old rotation.
    CHECK(!events.Read(101000).fresh);
    CHECK(events.Read(101000).sample.gyroscope[0] == 0.0f);
    CHECK(events.Read(30'000'000).valid); // Rest does not disconnect.
    CHECK(events.Read(30'000'000).sample.acceleration[1] == 1.0f);
    events.Rotation({180, 0, 0}, 2, 30'000'001);
    CHECK(events.Read(30'000'001).fresh); // Resume without another accelerometer event.
    CHECK(events.Read(30'000'001).sample.gyroscope[0] == 0.5f);
    events.Rotation({std::numeric_limits<double>::quiet_NaN(), 0, 0}, 3, 30'000'002);
    CHECK(events.Read(30'000'002).sample.gyroscope[0] == 0.5f);
    events.Rotation({90, 0, 0}, 1, 30'000'003); // Sensor clock reset.
    CHECK(events.Read(30'000'003).sample.gyroscope[0] == 0.25f);
    // Windows' face-up gravity must match the existing SDL controller convention.
    const auto face_up = Convert(0, 0, -1, 0, 0, 0);
    CHECK((face_up.acceleration == std::array<float, 3>{0, 1, 0}));
    const auto upright = Convert(0, -1, 0, 0, 0, 0);
    CHECK((upright.acceleration == std::array<float, 3>{0, 0, -1}));

    // Known rotations: one turn/second about X, quarter-turn about Y, half-turn
    // about Z.
    const auto rotation = Convert(0, 0, 0, 360, 90, -180);
    CHECK((rotation.gyroscope == std::array<float, 3>{1, 0.5f, 0.25f}));
    for (int i = -100; i <= 100; ++i) {
        const double x = static_cast<double>(i) / 100;
        const double y = std::sin(x);
        const double z = std::cos(x);
        const auto sample = Convert(x, y, z, 360 * x, 360 * y, 360 * z);
        const auto& a = sample.acceleration;
        const auto& g = sample.gyroscope;
        const double norm = x * x + y * y + z * z;
        CHECK(std::abs(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] - norm) < 0.000001);
        CHECK(std::abs(g[0] * g[0] + g[1] * g[1] + g[2] * g[2] - norm) < 0.000001);
    }

    constexpr std::int64_t first = 10'000'000;
    constexpr std::uint64_t nominal = 10'000;
    CHECK(DeltaMicroseconds(0, first, nominal) == nominal);
    CHECK(DeltaMicroseconds(first, first + 100'000, nominal) == 10'000);
    CHECK(DeltaMicroseconds(first, first + 200'000, nominal) == 20'000);
    CHECK(DeltaMicroseconds(first, first, nominal) == nominal);
    CHECK(DeltaMicroseconds(first, first - 1, nominal) == nominal);
    CHECK(DeltaMicroseconds(first, first + 1, nominal) == nominal);
    CHECK(DeltaMicroseconds(first, first + 1'000'000, nominal) == 100'000);
    CHECK(DeltaMicroseconds(first, first + 1'000'001, nominal) == 100'000);
    CHECK(DeltaMicroseconds(first, first + 10'000'000, nominal) == nominal);
    CHECK(DeltaMicroseconds(first, std::numeric_limits<std::int64_t>::max(), nominal) == nominal);
    CHECK(DeltaMicroseconds(first, std::numeric_limits<std::int64_t>::min(), nominal) == nominal);
    ReadingFreshness freshness;
    constexpr std::uint64_t timeout = 500'000;
    CHECK(!freshness.IsFresh(0, timeout));
    CHECK(!freshness.Observe(0, 0));
    CHECK(!freshness.Observe(-1, 0));
    CHECK(freshness.Observe(first, 0));
    CHECK(freshness.IsFresh(0, timeout));
    CHECK(freshness.IsFresh(timeout - 1, timeout));
    CHECK(!freshness.IsFresh(timeout, timeout));

    // Repeated polling and reopening must never refresh the cached sample.
    for (std::uint64_t now = timeout; now <= 10'000'000; now += nominal) {
        CHECK(!freshness.Observe(first, now));
        CHECK(!freshness.IsFresh(now, timeout));
    }
    // Stationary sensors in streaming mode remain fresh when only timestamps
    // change.
    for (std::uint64_t now = 10'000'000; now <= 20'000'000; now += nominal) {
        CHECK(freshness.Observe(first + static_cast<std::int64_t>(now * 10), now));
        CHECK(freshness.IsFresh(now, timeout));
    }
    // A reset of the sensor clock is accepted without waiting for its old value.
    CHECK(freshness.Observe(first, 20'010'000));
    CHECK(freshness.IsFresh(20'010'000, timeout));
    CHECK(!freshness.IsFresh(20'009'999, timeout));
    CHECK(!freshness.Observe(std::numeric_limits<std::int64_t>::min(), 20'020'000));
    CHECK(!freshness.IsFresh(20'510'000, timeout));

    std::cout << "Motion coordinate, unit, timestamp, and stale-reading checks "
                 "passed.\n";
}
