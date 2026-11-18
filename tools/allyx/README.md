# Native Windows motion for handhelds

Adds an optional C++/WinRT input engine for the built-in gyroscope and
accelerometer. ENABLE_WINDOWS_MOTION defaults on for Windows MSVC/clang-cl builds;
other platforms keep their existing input backends. Requires the Windows SDK
sensor headers and WindowsApp.lib.

In controller configuration, right-click a motion binding and choose
"Use built-in Windows motion (ROG Ally X)". Both sensors must be available.
Selecting the motion-only device changes motion bindings without replacing buttons.
Existing SDL and UDP motion remain available.

The backend uses sensor events, landscape coordinate conversion, timestamps,
stale-reading handling and clean sensor shutdown. The accompanying six-axis
service changes isolate start/stop state by handle and controller style.
EDEN_ALLYX_MOTION_TRACE=1 enables delivery diagnostics; it is not enabled by this PR.

## Checks and observed scope

Configure tools/allyx independently with CMake to build allyx_motion_checks and,
on supported Windows toolchains, allyx_sensor_probe. Test-SixAxisHandles.py builds
a synthetic fixture around the real six-axis service implementation. Its fixtures
are original test data; they contain no game data.

The same motion/input source was built in the Performance14 Windows x64 package.
The preceding Performance13 Direct ROG Ally X session recorded 462 delivery
reports; 244 had nonzero motion, with matching controller and guest peak values.
This confirms delivery in that session, not measured physical latency or universal
device compatibility. A complete build of this isolated contribution branch has
not yet been run. Other performance and renderer changes are excluded.

Prepared with AI assistance from the contributor's local Ally X development work.
