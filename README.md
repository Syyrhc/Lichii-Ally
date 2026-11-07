<!--
# SPDX-FileCopyrightText: Copyright 2025 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

# SPDX-FileCopyrightText: 2018 yuzu Emulator Project
# SPDX-License-Identifier: GPL-2.0-or-later
-->

<p align="center">
  <img src="dist/branding/lichii_icon_with_logo.png" alt="Lichii Ally" width="520">
</p>

<h1 align="center">Lichii Ally</h1>

<p align="center">
  <strong>Handheld-tuned edition of the Lichii Nintendo Switch and Switch 2 emulator for the ASUS ROG Xbox Ally X, ROG Ally, Lenovo Legion Go, MSI Claw, Steam Deck, and Windows Handheld PCs</strong>
</p>

<p align="center">
  <a href="https://github.com/ChrysthianChrisley/Lichii-Ally/releases/latest"><strong>Download Latest Release</strong></a>
</p>

---

**Lichii Ally** is a pre-configured, ready-to-run distribution of the open-source [Lichii](https://github.com/zortos293/Lichii) Nintendo Switch and Switch 2 emulator, engineered primarily around the **ASUS ROG Xbox Ally X** (`AMD Ryzen AI Z2 Extreme`, `Radeon 890M` RDNA 3.5, `24 GB LPDDR5X-8000`, and `120 Hz` `AMD FreeSync Premium VRR` display) while dynamically adapting to modern Windows gaming handhelds.

Combining the zero-setup usability of **Frizti v3** with upstream stability and renderer fixes from `nx2`, Lichii Ally ships with two architecture-specific packages (`AVX-512` and `Universal AVX2`) and automatic runtime detection of CPU core topology and physical RAM capacity (`16 GB` vs. `24 GB+`), providing out-of-the-box support across ASUS ROG Ally, Lenovo Legion Go, MSI Claw, Steam Deck, Ayaneo, GPD, and OneXPlayer devices.

## Downloads and Handheld Compatibility

Download the archive matching your processor architecture from the [Releases page](https://github.com/ChrysthianChrisley/Lichii-Ally/releases/latest), extract `Lichii-Ally`, and run `lichii.exe`:

| Handheld / Target Hardware | Processor Architecture | RAM | Recommended Package | Compiler Target |
|---|---|---|---|---|
| **ASUS ROG Xbox Ally X** | Ryzen AI Z2 Extreme (`Zen 5 + Zen 5c`) | 24 GB | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, `mtune=znver5`, ThinLTO + PGO) |
| **ASUS ROG Ally X / ROG Ally (Z1E & Z1)** | Ryzen Z1 Extreme / Z1 (`Zen 4`) | 16–24 GB | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, ThinLTO + PGO) |
| **Lenovo Legion Go / Go 2 / Go S (Z1E)** | Ryzen Z2 Extreme / Z1 Extreme (`Zen 5 / Zen 4`) | 16–32 GB | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, ThinLTO + PGO) |
| **Ayaneo / GPD / OneXPlayer (HX 370 / 8840U / 7840U)** | Ryzen AI 9 HX 370 / Ryzen 7 8840U (`Zen 5 / Zen 4`) | 16–64 GB | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, ThinLTO + PGO) |
| **ASUS ROG Xbox Ally (Base)** | Ryzen Z2 A (`Zen 2`, no AVX-512) | 16 GB | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |
| **Lenovo Legion Go S (Z2 Go)** | Ryzen Z2 Go (`Zen 3+`, no AVX-512) | 16–32 GB | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |
| **MSI Claw 8 AI+ / 7 AI+ / A1M** | Intel Core Ultra 200V / 100H (no AVX-512) | 16–32 GB | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |
| **Steam Deck LCD / OLED (Windows)** and other x64 PCs | Custom AMD `Zen 2` APU / x86-64 CPUs | 16 GB+ | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |

> **Important:** Processors without `AVX-512` support (Intel Core Ultra on MSI Claw, AMD `Zen 2` on Steam Deck and ROG Xbox Ally Base, and AMD `Zen 3+` on Legion Go S Z2 Go) **must** use the `Universal-AVX2.zip` package. Archives ending in `-debug-symbols-not-needed-to-play.zip` contain `.pdb` debugging symbols for crash analysis only and are not required for gameplay. If Windows prompts for Visual C++ runtime libraries on a clean installation, run `vc_redist.x64.exe` included inside the `Lichii-Ally` directory.

## Hardware-Specific Optimizations

1. **Windows 11 Hybrid Core Scheduling (`Zen 5 + Zen 5c` and Heterogeneous CPUs):**
   - Queries processor core `EfficiencyClass` topology via `GetLogicalProcessorInformationEx` and disables Windows *EcoQoS / Power Throttling* on critical emulation threads.
   - Dedicates full performance cores (such as **Zen 5 up to 5.0 GHz** on Ryzen AI Z2 Extreme / HX 370) to `GpuBack` and `GpuFront`, while isolating background Vulkan shader compilation workers onto efficiency cores (`Zen 5c` / E-cores) to prevent clock drops and frame-time spikes. Automatically adapts on homogeneous 8-core (`Zen 4`) and 4-core (`Zen 2` / `Zen 3+`) handhelds.
2. **Dynamic Unified Memory Allocation (16 GB vs. 24 GB+ APUs):**
   - Inspects host physical RAM at startup in `Device::CollectPhysicalMemoryInfo`: scales the Vulkan texture cache budget to **7 GiB** (Conservative) or **8 GiB** (Aggressive) with a **6,000-pipeline** ceiling on 24 GB+ handhelds (ROG Xbox Ally X, ROG Ally X, Legion Go 2, MSI Claw 8 AI+), while automatically maintaining a safe **4–6 GiB** budget and **3,500-pipeline** ceiling on 16 GB handhelds (ROG Ally Z1E, Legion Go 1, Steam Deck) so host memory is never exhausted.
3. **APU-Calibrated Asynchronous Shader Compilation:**
   - Caps concurrent Vulkan pipeline compilation workers on integrated GPUs to **6 threads** (down from 15), preventing CPU package power starvation and host RAM spikes on 15–35 W handhelds.
   - Stores pipeline caches in dedicated `vulkan-ally` files and automatically migrates existing `vulkan-frizti` or mainline `vulkan` caches on first launch.
4. **120 Hz VRR 40 FPS Mode & Live Frame-Rate Switching:**
   - Sets the default *The Legend of Zelda: Ocarina of Time* target frame rate to **40 FPS** (`25.0 ms`, preserving full native game speed via built-in Dynamic FPS), locking cleanly to every **3rd display refresh** on 120 Hz FreeSync VRR panels with low-latency phase alignment.
   - Supports instant in-game switching across **40 FPS**, **60 FPS**, and **30 FPS** via controller shortcut, keyboard hotkey, or the status bar selector.
5. **Windows 11 Xbox Full Screen Experience Integration:**
   - Launches games directly into **Borderless Windowed Fullscreen** by default on Windows to maintain seamless VRR engagement.
   - Binds default controller hotkeys to `View` and `Menu` stick-click combinations rather than the Xbox Guide button, preventing conflicts with Windows Xbox Game Bar.
6. **`clang-cl` 22 + ThinLTO + PGO + Upstream `nx2` Fixes:**
   - Compiled with LLVM 22.1.8, ThinLTO, and Profile-Guided Optimization (`lichii-windows.profdata`), incorporating automatic **DLSS to AMD FSR 3.1** upscaler replacement, Windows `SparseLargeVector` page commit stability (`#485`), expanded Turing macro RAM (`#439`), and clean GPU split teardown (`#438`).

## Recommended System Configuration (ROG Xbox Ally X)

| Component | Recommended Setting | Technical Rationale |
|---|---|---|
| **Armoury Crate SE — GPU Memory (VRAM)** | **8 GB** | Balances 8 GB dedicated to the Radeon 890M with 16 GB available for Windows, guest DRAM, and shader caches. |
| **Windows Display Settings** | **1080p @ 120 Hz + FreeSync (VRR) Enabled** | Enables exact `120 Hz / 3` Genlock cadence at 40 FPS and absorbs minor frame-time variance. |
| **AMD Software: Adrenalin Edition** | **Disable Radeon Anti-Lag, Radeon Boost, and AFMF** | Prevents driver-level frame queues from interfering with the emulator's internal low-latency host pacer. |
| **Operating Mode (TDP)** | **17W–20W** (40 FPS) / **25W–30W** (60 FPS) / **13W–15W** (30 FPS) | Matches APU package power to your selected target frame rate and 80 Wh battery life goal. |

## Controller and Keyboard Shortcuts

| Action | Controller Shortcut | Keyboard Shortcut |
|---|---|---|
| Cycle Target Frame Rate (**40 / 60 / 30 FPS**) | `View + R3` *(Right Stick Click)* | `F7` *(or status bar button)* |
| Toggle Borderless Fullscreen | `View + L3` *(Left Stick Click)* | `F11` |
| Toggle Performance Overlay | `Menu + R3` *(Right Stick Click)* | `Ctrl + V` |
| Pause / Resume Emulation | `Home + Plus` | `F4` |
| Stop Emulation | `L + Plus + Minus` | `F5` |

## Saves, Configuration, and Legal Notice

Lichii Ally uses the standard `%APPDATA%\eden` data directory, preserving existing save files, per-game settings, installed firmware, and game directories from Lichii, Eglantine, and Eden. Back up your save data regularly before testing updates.

Only use games, cryptographic keys, and firmware dumped legally from your own hardware. No games, keys, or firmware are included in this repository or its releases.

## Building from Source

Refer to [docs/Build.md](docs/Build.md) and [docs/Coding.md](docs/Coding.md) for prerequisite dependencies. To configure a release build targeting `x86-64-v4` (`AVX-512` with `znver5` instruction scheduling):

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DYUZU_BUILD_PRESET=v4
cmake --build build --target yuzu yuzu-cmd --parallel
```

## Credits and License

Lichii Ally builds upon the open-source work of:
- **[Lichii Ally](https://github.com/ChrysthianChrisley/Lichii-Ally)** maintained and optimized for Windows handhelds by **Chrys (ChrysthianChrisley)**
- **[Lichii](https://github.com/zortos293/Lichii)** by Zortos and contributors
- **[Frizti's Lichii Builds](https://github.com/Frizti/Lichii)** (zero-setup Ocarina of Time workflow, Dynamic FPS integration with **Dark**, and shader workarounds with **Evil**)
- **[Eglantine](https://github.com/Djihads80/Eglantine)** (Camille LaVey), **Eden**, **yuzu**, and **Sudachi**

Application branding artwork in [dist/branding](dist/branding) was created by **YOB (yobslab)**. Distributed under the **GPL-3.0-or-later** license; see [LICENSE.txt](LICENSE.txt) for full terms. This project is independent and is not affiliated with Nintendo, ASUS, or Microsoft.
