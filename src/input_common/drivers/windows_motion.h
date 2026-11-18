// SPDX-FileCopyrightText: 2026 Fritchii contributors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "input_common/input_engine.h"

namespace InputCommon {

class WindowsMotion final : public InputEngine {
public:
    explicit WindowsMotion(std::string input_engine_);
    ~WindowsMotion() override;

    [[nodiscard]] bool IsAvailable() const;
    std::vector<Common::ParamPackage> GetInputDevices() const override;
    MotionMapping GetMotionMappingForDevice(const Common::ParamPackage& params) override;
    Common::Input::ButtonNames GetUIName(const Common::ParamPackage& params) const override;

private:
    void Run(std::stop_token stop_token);
    bool WaitUntil(std::stop_token stop_token, std::chrono::steady_clock::time_point deadline);
    void ClearMotion();

    static constexpr PadIdentifier identifier{};
    std::atomic<bool> available{false};
    std::mutex wait_mutex;
    std::condition_variable_any wait_condition;
    std::jthread worker;
};

} // namespace InputCommon
