# ROG Xbox Ally X Tuning and Armoury Crate Configuration Guide

This guide details optimal system settings, Armoury Crate SE profiles, and memory tuning for running Switch 2 titles on the ASUS ROG Xbox Ally X (AMD Ryzen AI Z2 Extreme, Radeon 890M RDNA 3.5, 24 GB LPDDR5X-7500).

---

## 1. Memory Configuration (Crucial for Stability)

### GPU VRAM Allocation: Set to 6 GB (or 4 GB), Never 8 GB
* **Path:** `Armoury Crate SE -> Settings -> Operating Mode -> GPU Settings -> Memory Assigned to GPU`
* **Optimal Value:** **6 GB** (or **4 GB**)
* **Technical Reason:**
  - Switch 2 games allocate approximately **14,336 MiB (~14 GB)** of backing guest RAM.
  - Setting VRAM to 8 GB leaves only 16 GB for Windows 11. Once Windows (2.5 GB) and emulator runtime overhead (1.5 GB) are added, the system runs out of physical memory (commit exhaustion), causing aggressive disk paging and game freezes during scene transitions.
  - Setting VRAM to **6 GB** leaves **18 GB of physical RAM** for Windows, providing ample headroom for guest allocations.
  - Because AMD APUs feature Unified Memory Architecture (UMA), the Radeon 890M dynamically borrows host system memory for textures if needed without crashing or starving Windows.

### Windows Virtual Memory (Pagefile)
* **Path:** `System Properties -> Advanced -> Performance Settings -> Advanced -> Virtual Memory`
* **Setting:** Ensure **"Automatically manage paging file size for all drives"** is checked on your NVMe SSD (C:).

---

## 2. Armoury Crate SE Game Profile Settings

Create a dedicated Game Profile for `lichii.exe` in Armoury Crate SE:

| Category | Setting | Recommended Value | Technical Purpose |
|---|---|---|---|
| **Operating Mode** | Plugged-in (AC) | **25W - 30W Manual Profile** | Sustained 60 FPS in dense environments |
| **Operating Mode** | Battery (DC) | **17W - 20W Manual Profile** | Locked 40 FPS (120 Hz / 3rd refresh) with 2.5h+ battery life |
| **SPL (Sustained)** | Manual TDP | 25W | Baseline power draw |
| **sPPT (Short Burst)**| Manual TDP | 28W | Burst power for shader compiles and fast loading |
| **fPPT (Fast Burst)** | Manual TDP | 30W | Peak burst limit for initial asset decode |
| **Fan Curves** | Fan 1 & Fan 2 | 45% at 60C, 65% at 70C, 85% at 80C | Keeps APU below 75C without excessive acoustic noise |
| **Display Refresh** | Screen | **120 Hz** | Enables 40 FPS and 60 FPS VRR frame pacing |
| **AMD FreeSync** | Display | **Enabled** | Eliminates screen tearing and frame pacing judder |
| **Command Center FPS** | FPS Limiter | **Off** | Let Lichii's Dynamic FPS engine manage frame rates directly |

---

## 3. AMD Software (Adrenalin) Settings

Open AMD Software: Adrenalin Edition, select the `lichii.exe` profile, and apply:

* **AMD FreeSync:** `Enabled`
* **Radeon Anti-Lag:** `Disabled` (Lichii has low-latency fence synchronization built-in; external anti-lag causes pacing stutter).
* **Radeon Boost:** `Disabled` (conflicts with the emulator's internal dynamic resolution scaling).
* **Radeon Super Resolution (RSR):** `Disabled` (Lichii provides built-in FSR 3.1 and bilinear upscaling).
* **AMD Fluid Motion Frames (AFMF):** `Disabled` (frame generation is handled internally in Lichii).

---

## 4. Lichii In-App Settings for ROG Xbox Ally X

* **Console Mode:** `Handheld` (defaults to portable render target resolutions for +25% FPS).
* **VRAM Usage Mode:** `Conservative` (caps texture cache at 1,536 MiB expected / 2,048 MiB critical).
* **Anti-Aliasing:** `No sharpening` (fastest performance in foliage-heavy areas like Kokiri Forest).
* **Target Frame Rate:** `40 FPS` for handheld play or `60 FPS` when plugged in.
* **Max Resolution (built-in):** `Disabled` (allows Dynamic Resolution Scaling to reduce GPU load).
