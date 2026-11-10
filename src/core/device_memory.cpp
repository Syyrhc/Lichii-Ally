// SPDX-FileCopyrightText: Copyright 2020 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "core/device_memory.h"
#include "hle/kernel/board/nintendo/nx/k_system_control.h"

#ifdef _WIN32
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include "common/logging.h"
#include "common/memory_detect.h"
#include "common/thread.h"
#endif

namespace Core {

#ifdef HAS_NCE
constexpr size_t VirtualReserveSize = 1ULL << 38;
#else
constexpr size_t VirtualReserveSize = 1ULL << 39;
#endif

DeviceMemory::DeviceMemory()
    : buffer{Kernel::Board::Nintendo::Nx::KSystemControl::Init::GetIntendedMemorySize(),
             VirtualReserveSize} {}

DeviceMemory::~DeviceMemory() = default;

#ifdef _WIN32
void DeviceMemory::TrimZeroPagesOnStartup(std::stop_token stop) {
    using namespace std::chrono_literals;
    using Clock = std::chrono::steady_clock;
    constexpr size_t Busy = size_t{64} << 20;
    const u64 pressure_threshold = Common::GetMemInfo().TotalPhysicalMemory / 2;
    if (pressure_threshold == 0) {
        return;
    }
    Common::SetCurrentThreadName("ZeroTrim");
    Common::SetCurrentThreadPriority(Common::ThreadPriority::Low);
    const auto deadline = Clock::now() + 180s;
    std::mutex mutex;
    std::condition_variable_any wake;
    std::chrono::seconds interval = 5s;
    while (!stop.stop_requested()) {
        {
            std::unique_lock lock{mutex};
            wake.wait_until(lock, stop, (std::min)(Clock::now() + interval, deadline),
                            [] { return false; });
        }
        if (stop.stop_requested() || Clock::now() >= deadline) {
            return;
        }
        const auto before = Common::GetProcessMemoryUsage();
        if (before.system_commit_limit == 0 || before.available_physical >= pressure_threshold) {
            interval = 5s;
            continue;
        }
        const auto start = Clock::now();
        const auto zero_bytes = buffer.TrimZeroPages(before.working_set, stop);
        if (zero_bytes < Busy) {
            interval = (std::min)(interval * 2, 30s);
            continue;
        }
        interval = 5s;
        const auto after = Common::GetProcessMemoryUsage();
        LOG_INFO(HW_Memory,
                 "ZeroTrim found {} MiB of resident zero pages in {} ms; working set {} -> {} MiB; "
                 "available RAM {} -> {} MiB",
                 zero_bytes >> 20,
                 std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count(),
                 before.working_set >> 20, after.working_set >> 20,
                 before.available_physical >> 20, after.available_physical >> 20);
    }
}
#endif

} // namespace Core
