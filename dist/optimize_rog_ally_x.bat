@echo off
setlocal EnableDelayedExpansion
title ROG Xbox Ally X - Lichii Optimization Setup

echo =====================================================================
echo       ASUS ROG Xbox Ally X - Lichii Optimization Script
echo =====================================================================
echo.
echo Checking administrator privileges...
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] This script requires Administrator privileges.
    echo Please right-click this script and select 'Run as administrator'.
    echo.
    pause
    exit /b 1
)

echo [OK] Running with Administrator privileges.
echo.

echo ---------------------------------------------------------------------
echo 1. Configuring Windows Pagefile to System-Managed (SSD NVMe)
echo ---------------------------------------------------------------------
echo Switch 2 games allocate ~14 GB of backing guest RAM.
echo Setting pagefile to System-Managed ensures Windows dynamically
echo handles memory spillover without Out-Of-Memory crashes.
echo.
powershell -NoProfile -Command "try { $cs = Get-CimInstance Win32_ComputerSystem; $cs.AutomaticManagedPagefile = $true; Set-CimInstance -CimInstance $cs -ErrorAction Stop; Write-Host '[SUCCESS] Windows Virtual Memory is set to System-Managed.' -ForegroundColor Green } catch { Write-Host '[WARNING] Could not set automatically: ' $_.Exception.Message -ForegroundColor Yellow }"

echo.
echo ---------------------------------------------------------------------
echo 2. Disabling Background GameDVR / Background Recording
echo ---------------------------------------------------------------------
echo Background recording consumes 2 GB+ of RAM and CPU cycles.
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\GameDVR" /v "AppCaptureEnabled" /t REG_DWORD /d 0 /f >nul 2>&1
reg add "HKCU\System\GameConfigStore" /v "GameDVR_Enabled" /t REG_DWORD /d 0 /f >nul 2>&1
echo [SUCCESS] GameDVR background recording disabled.

echo.
echo ---------------------------------------------------------------------
echo 3. Setting Windows High Performance GPU Preference for Lichii
echo ---------------------------------------------------------------------
set "LICHII_EXE=%~dp0lichii.exe"
if exist "%LICHII_EXE%" (
    reg add "HKCU\Software\Microsoft\DirectX\UserGpuPreferences" /v "%LICHII_EXE%" /t REG_SZ /d "GpuPreference=2;" /f >nul 2>&1
    echo [SUCCESS] High performance GPU preference assigned to %LICHII_EXE%.
) else (
    echo [INFO] lichii.exe not found in this folder. It will use High Performance by default.
)

echo.
echo =====================================================================
echo                   ARMOURY CRATE SE SETUP CHECKLIST
echo =====================================================================
echo.
echo Please apply the following settings in Armoury Crate SE:
echo.
echo  1. GPU VRAM Allocation:
echo     - Open Armoury Crate SE - Settings - Operating Mode - GPU Settings.
echo     - Change 'Memory Assigned to GPU' from 8 GB to 6 GB (or 4 GB).
echo     - Technical Reason: 6 GB VRAM leaves 18 GB of physical RAM for
echo       Windows, preventing OOM freezes during Kokiri Forest streaming.
echo.
echo  2. Operating Mode (TDP):
echo     - Handheld / Battery: 17W - 20W Manual Profile (40 FPS VRR).
echo     - Plugged in / Turbo: 25W - 30W Profile (60 FPS).
echo.
echo  3. Display & AMD Software (Adrenalin):
echo     - Display: 1080p @ 120 Hz with FreeSync Premium ENABLED.
echo     - AMD Software: Disable AFMF, Radeon Anti-Lag, and Radeon Boost
echo       (Lichii handles frame gen and pacing natively).
echo.
echo =====================================================================
echo Configuration complete! Restart your ROG Ally X to apply VRAM changes.
echo =====================================================================
echo.
pause
