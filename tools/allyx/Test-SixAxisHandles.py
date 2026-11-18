"""Exercise the guest six-axis service with synthetic controller input and real HID shared memory.

Run in the project's Windows LLVM/MSVC environment:
    python Test-SixAxisHandles.py OUTPUT_DIRECTORY
"""
from pathlib import Path
import subprocess
import sys

root = Path(__file__).resolve().parents[2]
hid = root / "src/hid_core"
out = Path(sys.argv[1]).resolve()
out.mkdir(parents=True, exist_ok=True)

# Use the real service declaration, service implementation and input sample type. Only OS-facing
# collaborators are substituted, so handle IPC, routing and writes all run as they do in Lichii.
service_cpp = (hid / "resources/six_axis/six_axis.cpp").read_text()
service_h = (hid / "resources/six_axis/six_axis.h").read_text()
utils = (hid / "hid_util.h").read_text()
controller = (hid / "frontend/emulated_controller.h").read_text()
motion_begin = controller.index("struct ControllerMotion {")
motion = controller[motion_begin:controller.index("\n};", motion_begin) + 3]
validate = utils[utils.index("constexpr bool IsNpadIdValid"):utils.index("constexpr Result IsVibrationHandleValid")]
convert = utils[utils.index("constexpr size_t NpadIdTypeToIndex"):utils.index("constexpr Core::HID::NpadStyleSet")]

fixture = r'''
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <string_view>
#include "hid_core/resources/shared_memory_format.h"
#include "hid_core/resources/six_axis/sensor_info.h"
#define ASSERT assert
#define LOG_ERROR(...) ((void)0)
#define LOG_INFO(category, ...) Record(__VA_ARGS__)

struct Result {
    int raw{};
    constexpr bool IsError() const { return raw != 0; }
};
constexpr Result ResultSuccess{}, ResultInvalidNpadId{1}, NpadDeviceIndexOutOfRange{2},
                 InvalidSixAxisFusionRange{3};

namespace Settings {
struct {
    struct { bool enabled{true}; bool GetValue() const { return enabled; } } motion_enabled;
} values;
}
namespace Core::Timing { struct CoreTiming {}; }
namespace Core::HID {
''' + motion + r'''
class EmulatedController {
public:
    NpadStyleIndex style{NpadStyleIndex::None};
    std::array<ControllerMotion, 2> motion{};
    NpadStyleIndex GetNpadStyleIndex() const { return style; }
    bool IsConnected() const { return style != NpadStyleIndex::None; }
    const auto& GetMotions() const { return motion; }
    void SetGyroscopeZeroDriftMode(GyroscopeZeroDriftMode) {}
};
class HIDCore {
public:
    std::array<EmulatedController, 10> pads{};
    auto* GetEmulatedControllerByIndex(std::size_t i) { return &pads[i]; }
};
}
namespace Service::HID {
''' + validate + convert + r'''
constexpr std::size_t AruidIndexMax = 1;

struct Applet {
    struct {
        struct { bool is_assigned{true}, enable_six_axis_sensor{true}; } flag;
        u64 aruid{15};
        SharedMemoryFormat* shared_memory_format{};
    } data;
    auto* GetAruidDataByIndex(std::size_t) { return &data; }
};
class ControllerBase {
public:
    explicit ControllerBase(Core::HID::HIDCore& core) : hid_core{core} {}
    virtual ~ControllerBase() = default;
    virtual void OnInit() = 0;
    virtual void OnRelease() = 0;
    virtual void OnUpdate(const Core::Timing::CoreTiming&) = 0;
    bool IsControllerActivated() const { return true; }
    std::shared_ptr<Applet> applet_resource = std::make_shared<Applet>();
    std::recursive_mutex mutex;
    std::recursive_mutex* shared_mutex = &mutex;
    Core::HID::HIDCore& hid_core;
};
unsigned requests{}, deliveries{}, stopped_samples{}, disabled_applets{};
float largest_guest_peak{};
void Record(const char*, const char*, u8, Core::HID::NpadStyleIndex, Core::HID::DeviceIndex) {
    ++requests;
}
void Record(const char*, u64, Core::HID::NpadIdType, Core::HID::NpadStyleIndex, std::size_t,
            std::string_view gate, s64, u32, float, float guest_peak) {
    ++deliveries;
    largest_guest_peak = std::max(largest_guest_peak, guest_peak);
    if (gate == "handle stopped") {
        ++stopped_samples;
        assert(guest_peak == 0);
    }
    if (gate == "applet disabled") ++disabled_applets;
}
}
'''
tests = r'''
int main(int argc, char**) {
    using namespace Service::HID;
    using enum Core::HID::NpadStyleIndex;
    using Index = Core::HID::DeviceIndex;
    using Handle = Core::HID::SixAxisSensorHandle;
    const bool tracing = argc > 1;
    assert(_putenv_s("EDEN_ALLYX_MOTION_TRACE", tracing ? "1" : "") == 0);
    Core::HID::HIDCore core;
    SixAxis service{core, {}};
    auto memory = std::make_unique<SharedMemoryFormat>();
    service.applet_resource->data.shared_memory_format = memory.get();
    auto& device = core.pads[0];
    auto& rings = memory->npad.npad_entry[0].internal_state;
    auto tick = [&]() { service.OnUpdate({}); };
    auto enable = [&](Handle h, bool on) { assert(!service.SetSixAxisEnabled(h, on).IsError()); };
    auto rest = [&](Handle h) {
        bool value{};
        assert(!service.IsSixAxisSensorAtRest(h, value).IsError());
        return value;
    };
    auto gyro = [](auto& ring) { return ring.lifo.ReadCurrentEntry().state.gyro; };
    const Handle full{Fullkey, 0, Index::None}, left{JoyconDual, 0, Index::Left},
                 right{JoyconDual, 0, Index::Right}, single{JoyconRight, 0, Index::Right};
    device.motion[0].gyro = {0.25f, 0, 0};
    device.motion[0].is_at_rest = false;
    device.motion[1].gyro = {0, 0.5f, 0};
    device.motion[1].is_at_rest = true;
    device.style = Fullkey;
    tick();
    assert(gyro(rings.sixaxis_fullkey_lifo)[0] == 0.25f && !rest(full));
    enable(right, false);
    enable(right, false);
    assert(requests == (tracing ? 1u : 0u));
    const Handle raw_pokeball{Pokeball, 0, Index::None};
    enable(raw_pokeball, false);
    tick();
    assert(gyro(rings.sixaxis_fullkey_lifo)[0] == 0.25f && !rest(full) && rest(raw_pokeball));
    device.style = Pokeball;
    tick();
    assert(gyro(rings.sixaxis_fullkey_lifo)[0] == 0.25f);
    assert(rings.sixaxis_fullkey_lifo.lifo.ReadCurrentEntry().state.delta_time == 15000000);
    enable(full, false);
    tick();
    assert(gyro(rings.sixaxis_fullkey_lifo)[0] == 0 && rest(full));
    enable(full, true);
    device.style = JoyconDual;
    tick();
    assert(gyro(rings.sixaxis_dual_left_lifo)[0] == 0.25f && !rest(left));
    assert(gyro(rings.sixaxis_dual_right_lifo)[1] == 0 && rest(right));
    enable(right, true);
    enable({JoyconDual, 0, Index::None}, false);
    assert(service.SetSixAxisEnabled({JoyconDual, 0, Index::MaxDeviceIndex}, false).IsError());
    assert(service.SetSixAxisEnabled({JoyconDual, 0xff, Index::Left}, false).IsError());
    enable(full, false);
    enable(single, false);
    tick();
    assert(gyro(rings.sixaxis_dual_right_lifo)[1] == 0.5f && rest(right));
    assert(gyro(rings.sixaxis_dual_left_lifo)[0] == 0.25f && !rest(left));
    Settings::values.motion_enabled.enabled = false;
    tick();
    assert(gyro(rings.sixaxis_dual_left_lifo)[0] == 0 && rest(left));
    Settings::values.motion_enabled.enabled = true;
    const auto sequence = rings.sixaxis_dual_left_lifo.lifo.ReadCurrentEntry().state.sampling_number;
    service.applet_resource->data.flag.enable_six_axis_sensor = false;
    tick();
    assert(rings.sixaxis_dual_left_lifo.lifo.ReadCurrentEntry().state.sampling_number == sequence);
    service.applet_resource->data.flag.enable_six_axis_sensor = true;
    tick();
    assert(rings.sixaxis_dual_left_lifo.lifo.ReadCurrentEntry().state.sampling_number == sequence + 1);
    device.style = Fullkey;
    tick();
    assert(gyro(rings.sixaxis_fullkey_lifo)[0] == 0 && rest(full));
    enable(full, true);
    tick();
    assert(gyro(rings.sixaxis_fullkey_lifo)[0] == 0.25f);
    const auto deliveries_before = deliveries;
    for (unsigned i = 0; i < 2000; ++i) tick();
    assert(deliveries - deliveries_before == (tracing ? 1u : 0u));
    device.style = JoyconRight;
    tick();
    assert(gyro(rings.sixaxis_right_lifo)[1] == 0);
    enable(single, true);
    tick();
    assert(gyro(rings.sixaxis_right_lifo)[1] == 0.5f);
    device.style = None;
    core.pads[8].style = Handheld;
    core.pads[8].motion[0].gyro = {0, 0, 0.75f};
    tick();
    assert(gyro(memory->npad.npad_entry[8].internal_state.sixaxis_handheld_lifo)[2] == 0.75f);
    assert(largest_guest_peak == (tracing ? 0.75f : 0.0f));
    assert((deliveries > 0) == tracing && (stopped_samples > 0) == tracing);
    assert(disabled_applets == (tracing ? 1u : 0u));
    std::puts("PASS: handle isolation, rest, invalid IDs, shared memory, input gating and trace");
}
'''
cpp = out / "six-axis-handles.cpp"
exe = out / "six-axis-handles.exe"
cpp.write_text(fixture + service_h[service_h.index("namespace Core::HID"):]
               + service_cpp[service_cpp.index("namespace Service::HID"):] + tests)
subprocess.run(["clang-cl", "/nologo", "/std:c++20", "/EHsc", "/O2", "/UNDEBUG",
                "/D_CRT_SECURE_NO_WARNINGS",
                "/I" + str(root / "src"), str(cpp), "/Fe:" + str(exe),
                "/Fo:" + str(out / "six-axis-handles.obj")], check=True)
subprocess.run([str(exe)], check=True, timeout=20)
subprocess.run([str(exe), "trace"], check=True, timeout=20)
