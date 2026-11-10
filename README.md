<!--
# SPDX-FileCopyrightText: Copyright 2025 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

# SPDX-FileCopyrightText: 2018 yuzu Emulator Project
# SPDX-License-Identifier: GPL-2.0-or-later
-->

<p align="center">
  <img src="dist/branding/lichii_ally_1024.png" alt="Lichii Ally" width="320">
</p>

<h1 align="center">Lichii Ally</h1>

<p align="center">
  <strong>Handheld-tuned edition of the Lichii Nintendo Switch and Switch 2 emulator for the ASUS ROG Xbox Ally X, ROG Ally, Lenovo Legion Go, MSI Claw, Steam Deck, and Windows Handheld PCs</strong>
</p>

<p align="center">
  <a href="https://github.com/Syyrhc/Lichii-Ally/releases/latest"><strong>Download Latest Release</strong></a>
</p>

---

**Lichii Ally** is a pre-configured, ready-to-run distribution of the open-source [Lichii](https://github.com/zortos293/Lichii) Nintendo Switch and Switch 2 emulator, engineered primarily around the **ASUS ROG Xbox Ally X** (`AMD Ryzen AI Z2 Extreme`, `Radeon 890M` RDNA 3.5, `24 GB LPDDR5X-8000`, and `120 Hz` `AMD FreeSync Premium VRR` display) while dynamically adapting to modern Windows gaming handhelds.

Combining the zero-setup usability of **Frizti v3**, the AMD RDNA and *Ocarina of Time* performance breakthroughs of **[CheezerWheezer/Lichii](https://github.com/CheezerWheezer/Lichii)** (`amd-superior`) and **[conongnang/Lichii](https://github.com/conongnang/Lichii)** (`oot-amd-perf`), and upstream stability fixes from `nx2`, Lichii Ally ships with two architecture-specific packages (`AVX-512` and `Universal AVX2`) and automatic runtime detection of CPU core topology and physical RAM capacity (`16 GB` vs. `24 GB+`), providing out-of-the-box support across ASUS ROG Ally, Lenovo Legion Go, MSI Claw, Steam Deck, Ayaneo, GPD, and OneXPlayer devices.

## Downloads and Handheld Compatibility

Download the archive matching your processor architecture from the [Releases page](https://github.com/Syyrhc/Lichii-Ally/releases/latest), extract `Lichii Ally <version>`, and run `lichii.exe`:

| Handheld / Target Hardware | Processor Architecture | RAM | Recommended Package | Compiler Target |
|---|---|---|---|---|
| **ASUS ROG Xbox Ally X** | Ryzen AI Z2 Extreme (`Zen 5 + Zen 5c`) | 24 GB | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, `mtune=znver5`, ThinLTO + PGO) |
| **ASUS ROG Ally X / ROG Ally (Z1E & Z1)** | Ryzen Z1 Extreme / Z1 (`Zen 4`) | 16-24 GB | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, ThinLTO + PGO) |
| **Lenovo Legion Go / Go 2 / Go S (Z1E)** | Ryzen Z2 Extreme / Z1 Extreme (`Zen 5 / Zen 4`) | 16-32 GB | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, ThinLTO + PGO) |
| **Ayaneo / GPD / OneXPlayer (HX 370 / 8840U / 7840U)** | Ryzen AI 9 HX 370 / Ryzen 7 8840U (`Zen 5 / Zen 4`) | 16-64 GB | `Lichii-Ally-vX-Xbox-Ally-X-AVX512.zip` | `x86-64-v4` (`AVX-512`, ThinLTO + PGO) |
| **ASUS ROG Xbox Ally (Base)** | Ryzen Z2 A (`Zen 2`, no AVX-512) | 16 GB | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |
| **Lenovo Legion Go S (Z2 Go)** | Ryzen Z2 Go (`Zen 3+`, no AVX-512) | 16-32 GB | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |
| **MSI Claw 8 AI+ / 7 AI+ / A1M** | Intel Core Ultra 200V / 100H (no AVX-512) | 16-32 GB | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |
| **Steam Deck LCD / OLED (Windows)** and other x64 PCs | Custom AMD `Zen 2` APU / x86-64 CPUs | 16 GB+ | `Lichii-Ally-vX-Universal-AVX2.zip` | `x86-64-v3` (`AVX2`, ThinLTO + PGO) |

> **Important:** Processors without `AVX-512` support (Intel Core Ultra on MSI Claw, AMD `Zen 2` on Steam Deck and ROG Xbox Ally Base, and AMD `Zen 3+` on Legion Go S Z2 Go) **must** use the `Universal-AVX2.zip` package. Archives ending in `-debug-symbols-not-needed-to-play.zip` contain `.pdb` debugging symbols for crash analysis only and are not required for gameplay. If Windows prompts for Visual C++ runtime libraries on a clean installation, run `vc_redist.x64.exe` included inside the `Lichii-Ally` directory.

## Handheld Game Performance and Compatibility

Performance verified on real handheld hardware running Windows 11 with **Lichii Ally v6.0** (`Xbox-Ally-X-AVX512` build, Handheld mode, Vulkan backend):

| Game Title | Program ID | Tested Handheld | Processor / GPU / RAM | Verified Frame Rate | Notes |
|---|---|---|---|---|---|
| **The Legend of Zelda: Ocarina of Time** | `0400486024DEC000` | ASUS ROG Xbox Ally X | Ryzen AI Z2 Extreme / Radeon 890M / 24 GB | **60 FPS** *(intro & cutscenes)* / **40 FPS** *(Kokiri Forest & gameplay)* | Built-in Dynamic FPS, DRS bilinear neural bypass, and physical RAM release applied automatically |
| **Pokemon Pokopia** | `0400A3A01EE4C000` | ASUS ROG Xbox Ally X | Ryzen AI Z2 Extreme / Radeon 890M / 24 GB | **40-60 FPS** | Playable in-game with smooth frame pacing |
| **Donkey Kong Bananza** | `040074A01BF12000` | ASUS ROG Xbox Ally X | Ryzen AI Z2 Extreme / Radeon 890M / 24 GB | **12-30 FPS** | Verified on v2; dedicated optimizations and automatic per-game profiles planned for upcoming releases |

> **Roadmap Note:** Upcoming versions of Lichii Ally will introduce automatic **Per-Game Optimization Profiles** (applying title-specific console mode, frame pacing, buffer preflushing, and shader/texture cache presets automatically based on Program ID).

### Submit Your Handheld Benchmark

Testing on an ASUS ROG Xbox Ally X, ROG Ally (Z1E / X), Lenovo Legion Go, MSI Claw, Steam Deck, or another Windows handheld? Share your in-game FPS results to help expand the compatibility table:

- **[Submit a Handheld FPS Benchmark Report](https://github.com/Syyrhc/Lichii-Ally/issues/new?title=%5BBenchmark%5D+Game+Name+-+Handheld+Model&body=**Handheld+Model%3A**+%0A**Processor+%2F+RAM%3A**+%0A**Lichii+Ally+Package%3A**+Xbox-Ally-X-AVX512+or+Universal-AVX2%0A**Game+Title+%26+Program+ID%3A**+%0A**Average+FPS%3A**+%0A**TDP+%2F+Power+Mode%3A**+%0A**Console+Mode%3A**+Docked+or+Handheld%0A**Notes%3A**+)**

## Hardware-Specific Optimizations

1. **Windows 11 Hybrid Core Scheduling (`Zen 5 + Zen 5c` and Heterogeneous CPUs):**
   - Queries processor `EfficiencyClass` topology via Windows CPU Sets (`GetSystemCpuSetInformation` and `SetThreadSelectedCpuSets`). On hybrid CPUs with fewer than 8 performance logical processors (such as the ROG Xbox Ally X's `3x Zen 5 + 5x Zen 5c`), time-critical emulation threads run unrestricted across all 16 logical processors so the OS Hetero Scheduler uses both `Zen 5` and `Zen 5c` cores without crowding, while background shader compilation workers are steered to `Zen 5c` efficiency cores.
   - Dedicates isolated cores to `GpuFront` and `GpuBack` on processors with 6 or more physical performance cores (such as the 8-core `Zen 4` Ryzen Z1 Extreme).
2. **UMA-Safe Memory & Texture Cache Management:**
   - Calibrates the integrated GPU memory budget (`4 GiB` Conservative / `6 GiB` Aggressive) and automatically recompresses Switch 2 ASTC textures to **BC3** (`4x` lower VRAM footprint than uncompressed RGBA8 on GPUs without native ASTC hardware), keeping `SparseLayers` (`384 MiB`) and `TrimmedAstcArray` (`256 MiB`) pools within handheld BIOS UMA carveouts without exhausting Windows host RAM.
   - Skips uploading stale guest memory into newly recreated GPU-written images (+20% FPS in Castle Town), batches texture garbage-collector downloads into a single GPU wait, skips redundant uploads overwritten by join copies, and releases Switch 2 guest RAM cleanly when stopping emulation.
   - Stores pipeline caches in dedicated `vulkan-ally` files (`6,000` pipeline retention ceiling on 24 GB+ handhelds, `3,500` on 16 GB handhelds) and automatically migrates existing `vulkan-frizti` or mainline `vulkan` caches on first launch.
3. **AMD RDNA 3 / 3.5 Renderer, `VK_KHR_pipeline_binary` & Shader Optimizations:**
   - **Persistent AMD Compiled Shader Binary Cache (`VK_KHR_pipeline_binary`):** AMD's Windows Vulkan driver does not persist compiled pipelines across sessions on its own. Lichii Ally saves compiled AMD RDNA ISA pipeline binaries to disk (`vulkan_binaries.fork-v54.bin`) and reloads them in `0.2-0.4 ms` per pipeline, cutting *Ocarina of Time* boot compilation time from minutes to seconds and accepting shared shader caches built on NVIDIA or other GPUs.
   - **Ocarina of Time CPU-Read Page Preflushing & Bounded Compute Write Marks:** Preflushes *Ocarina of Time*'s ~200 CPU-read GPU pages per second at fences instead of stalling the CPU and GPU threads on synchronous read traps, arms read traps only on pages not yet armed, and marks Switch 2 compute storage buffers as GPU-written only as far as the shader's stores reach.
   - Includes non-blocking AMD occlusion query synchronization (`VK_QUERY_RESULT_WITH_AVAILABILITY_BIT`), `512`-draw command submission batching on AMD proprietary drivers, deferred background compilation for software-matrix compute shaders, `16x16x16` `VK_KHR_cooperative_matrix` tensor acceleration (`HMMA`) on RDNA 3 / 3.5, multithreaded texture unswizzling (`LinePool`), and automatic **DLSS to AMD FSR 3.1** upscaler replacement.
4. **120 Hz VRR 40 FPS Mode, Native Handheld Mode & Automatic Gamepad Binding:**
   - Defaults to **Handheld Console Mode (`HANDHELD`)**, instructing Switch and Switch 2 titles to use their portable internal resolution and graphical presets (lighter shadows, effects, and render targets), delivering **20% to 35% higher FPS** and lower VRAM consumption on the ROG Xbox Ally X.
   - Sets the default *The Legend of Zelda: Ocarina of Time* target frame rate to **40 FPS** (`25.0 ms`, preserving full native game speed via built-in Dynamic FPS), locking cleanly to every **3rd display refresh** on 120 Hz FreeSync VRR panels.
   - Automatically detects and binds the built-in handheld controller (`Xbox 360 Controller 0`) to Player 1 and Handheld profiles on first launch.
5. **Windows 11 Xbox Full Screen Experience Integration:**
   - Launches games directly into **Borderless Windowed Fullscreen** by default on Windows to maintain seamless VRR engagement.
   - Binds default controller hotkeys to `View` and `Menu` stick-click combinations rather than the Xbox Guide button, preventing conflicts with Windows Xbox Game Bar.
6. **`clang-cl` 22 + ThinLTO + PGO + Upstream `nx2` Fixes:**
   - Compiled with LLVM 22.1.8, ThinLTO, and Profile-Guided Optimization (`lichii-windows.profdata`), incorporating non-blocking Switch 2 asynchronous pipeline skipping (`NativeAsyncDraws::Skip`), *Ocarina of Time* anti-aliasing tensor network bypass (`No sharpening` by default for +20% Kokiri Forest FPS), delayed puller fence release, AMD compare-sampler and blend state fixes, Turing macro JIT fallback recovery, and AV1 cutscene audio/video synchronization.

## Recommended System Configuration (ROG Ally & ROG Xbox Ally X)

Follow this complete checklist to maximize FPS, avoid out-of-memory stalls, and lock in smooth 120 Hz VRR frame pacing on ASUS ROG Ally handhelds.

### 1. ASUS Armoury Crate SE & BIOS Settings
| Setting | ROG Xbox Ally X (24 GB) | ROG Ally Z1E / Z1 (16 GB) | Why This Matters |
|---|---|---|---|
| **GPU Memory Assigned to GPU (VRAM)** | **6 GB** *(recommended)* or **8 GB** | **4 GB** | On 24 GB models, 6 GB VRAM leaves **18 GB** of system RAM for Windows 11, guest memory, and shader compilation. Always restart the Ally after changing VRAM. |
| **Control Mode (Command Center)** | **Gamepad Mode** *(or Auto)* | **Gamepad Mode** | Lichii Ally v6.2 auto-detects and maps `Xbox 360 Controller 0` to Handheld mode on launch when Gamepad Mode is active. |
| **Operating Mode / Manual TDP** | **17W-20W** *(40 FPS)* / **25W-30W** *(60 FPS+)* / **13W-15W** *(30 FPS)* | **18W-25W** *(Performance / Turbo)* | RDNA 3.5 efficiency peaks at 17W-20W for 40 FPS (`120 Hz / 3` cadence). Use 25W+ when targeting 60 FPS or high-refresh modes. |
| **Refresh Rate (Command Center)** | **120 Hz** at **1080p** | **120 Hz** at **1080p** | Leaving the panel at 60 Hz prevents proper VRR Genlock engagement at 40 FPS and 80+ FPS. |

### 2. Windows 11 Display & Power Settings
- **Enable 120 Hz Refresh Rate:** Go to **Settings > System > Display > Advanced display** and set **Choose a refresh rate** to **120.00 Hz**.
- **Enable Variable Refresh Rate (VRR):** Go to **Settings > System > Display > Graphics > Change default graphics settings** and turn **Variable Refresh Rate** **ON**.
- **Page File (Virtual Memory):** Leave Windows page file on **System Managed** (at least 16-32 GB capacity available on SSD) so Windows commit limit stays well above Switch 2 virtual reservations.

### 3. AMD Software: Adrenalin Edition
Open **AMD Software > Settings (Gear Icon) > Display & Gaming** and apply:
- **AMD FreeSync Premium:** **Enabled** (critical for smooth frame presentation at 40 FPS and variable frame rates).
- **Radeon Anti-Lag:** **Disabled** (conflicts with the emulator's internal low-latency frame pacer and causes micro-stuttering).
- **Radeon Boost:** **Disabled**.
- **AMD Fluid Motion Frames (AFMF):** **Disabled** (disrupts frame pacing and introduces HUD artifacts).
- **Radeon Chill / Frame Rate Target Control (FRTC):** **Disabled** (let Lichii Ally manage frame caps).
- **Shader Cache:** Keep AMD Driver Shader Cache enabled (default **AMD Optimized**).

### 4. Lichii Ally v6.2 Recommended Emulator Settings
These settings are pre-configured automatically on first launch, and can be verified under **Emulation > Configure**:

| Category | Setting | Recommended Value | Technical Rationale |
|---|---|---|---|
| **System** | **Console Mode** | **Handheld** | Reduces internal render targets and heavy effects for +20% to +35% higher FPS and lower VRAM use on 7" handheld screens. |
| **Core** | **Memory Layout** | **12 GiB (Switch 2)** / **6 GiB (Switch 1)** | Switch 2 titles require the 12 GiB guest heap layout (`9809 MiB` app pool) to avoid guest allocator NULL crashes; `EDEN_NX2_SEC_RESERVE` commits physical RAM only as pages are touched. |
| **Graphics** | **Backend / GPU Accuracy** | **Vulkan** / **Low (Normal)** | High/Accurate is no longer needed since v6.x compiles all 120,461 shaders for AMD RDNA ahead of time. |
| **Graphics** | **VSync Mode & Async Present** | **Mailbox** + **Async Presentation ON** | Delivers frames instantaneously to the 120 Hz FreeSync display with minimal input lag. |
| **Graphics** | **ASTC Recompression** | **BC3 (Medium)** | Cuts VRAM bandwidth pressure by ~4x on APUs without native ASTC hardware decoders. |
| **Graphics** | **Ocarina of Time Anti-Aliasing** | **No sharpening** *(max FPS)* or **FSR 3.1 / FSR 4** | Bypasses the game's heavy neural AA pass for maximum FPS, or switches cleanly to AMD FSR. |
| **Graphics > Adv.** | **Mesh Shaders** | **Automatic** *(Compute Fallback on AMD)* | Uses AMD compute fallback (`#814`) to prevent RDNA driver hangs on mesh shader draws. |
| **Controls** | **Player 1 / Handheld** | **Xbox 360 Controller 0** | Auto-mapped on boot even when upgrading from older Keyboard Only configs. |

## Controller and Keyboard Shortcuts

| Action | Controller Shortcut | Keyboard Shortcut |
|---|---|---|
| Cycle Target Frame Rate (**40 / 60 / 30 FPS**) | `View + R3` *(Right Stick Click)* | `F7` *(or status bar button)* |
| Toggle Live High Refresh Rate (*Ocarina of Time* >60 FPS) | - | `Ctrl + U` |
| Toggle Borderless Fullscreen | `View + L3` *(Left Stick Click)* | `F11` |
| Toggle Performance Overlay (FPS / RAM / VRAM) | `Menu + R3` *(Right Stick Click)* | `Ctrl + V` |
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
- **[Lichii](https://github.com/zortos293/Lichii)** by Zortos and contributors
- **[CheezerWheezer's AMD-Superior Lichii Fork](https://github.com/CheezerWheezer/Lichii)** (`VK_KHR_pipeline_binary` compiled shader binary persistence on AMD Windows drivers and cross-driver shader cache compatibility)
- **[conongnang's Ocarina of Time AMD Performance Fork](https://github.com/conongnang/Lichii)** (Ocarina of Time CPU-read GPU page preflushing, texture cache upload/eviction optimizations, bounded native compute storage write marks, and AMD RDNA pipeline improvements)
- **[Frizti's Lichii Builds](https://github.com/Frizti/Lichii)** (zero-setup Ocarina of Time workflow, Dynamic FPS integration with **Dark**, and shader workarounds with **Evil**)
- **[Eglantine](https://github.com/Djihads80/Eglantine)** (Camille LaVey), **Eden**, **yuzu**, and **Sudachi**

Application branding artwork in [dist/branding](dist/branding) was created by **YOB (yobslab)**. Distributed under the **GPL-3.0-or-later** license; see [LICENSE.txt](LICENSE.txt) for full terms. This project is independent and is not affiliated with Nintendo, ASUS, or Microsoft.
