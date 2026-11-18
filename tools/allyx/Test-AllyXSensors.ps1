# SPDX-License-Identifier: GPL-3.0-or-later
#requires -Version 5.1
<#
.SYNOPSIS
Shows the Windows gyroscope and accelerometer readings used by native motion.
.EXAMPLE
powershell.exe -NoProfile -File .\tools\allyx\Test-AllyXSensors.ps1
#>
[CmdletBinding()]
param(
    [ValidateRange(2, 60)]
    [int]$Seconds = 10
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 2.0
if ($env:OS -ne 'Windows_NT' -or $PSVersionTable.PSEdition -ne 'Desktop' -or
    -not [Environment]::Is64BitProcess) {
    throw 'Use 64-bit Windows PowerShell 5.1 (powershell.exe), not PowerShell 7 (pwsh.exe), for this diagnostic.'
}

try {
    $system = Get-CimInstance Win32_ComputerSystem
    $cpu = Get-CimInstance Win32_Processor | Select-Object -First 1
    Write-Host ('Model: {0}; CPU: {1}; RAM visible to Windows: {2:F1} GiB' -f
        $system.Model, $cpu.Name, ($system.TotalPhysicalMemory / 1GB))
} catch {
    Write-Warning 'System information was unavailable; continuing with the sensor check.'
}

$gyro = [Windows.Devices.Sensors.Gyrometer, Windows.Devices.Sensors, ContentType = WindowsRuntime]::GetDefault()
$accel = [Windows.Devices.Sensors.Accelerometer, Windows.Devices.Sensors, ContentType = WindowsRuntime]::GetDefault()
if ($null -eq $gyro -or $null -eq $accel) {
    Write-Host ('Gyroscope present:     ' + ($null -ne $gyro))
    Write-Host ('Accelerometer present: ' + ($null -ne $accel))
    throw 'Windows must expose both sensors. Check the ASUS sensor drivers in Device Manager, then rerun this diagnostic.'
}

$oldGyroInterval = $gyro.ReportInterval
$oldAccelInterval = $accel.ReportInterval
$gyroSamples = 0
$accelSamples = 0
$lastGyroTimestamp = $null
$lastAccelTimestamp = $null
$maximumGyroSpeed = 0.0
try {
    $gyro.ReportInterval = [uint32][Math]::Max(10, $gyro.MinimumReportInterval)
    $accel.ReportInterval = [uint32][Math]::Max(10, $accel.MinimumReportInterval)
    Write-Host ('Gyroscope minimum interval:     {0} ms' -f $gyro.MinimumReportInterval)
    Write-Host ('Accelerometer minimum interval: {0} ms' -f $accel.MinimumReportInterval)
    Write-Host "Gently tilt the Ally X for $Seconds seconds. Gyro is degrees/second; accelerometer is g."
    Write-Host 'Values are raw Windows axes; the emulator applies its controller coordinate conversion.'
    $timer = [Diagnostics.Stopwatch]::StartNew()
    while ($timer.Elapsed.TotalSeconds -lt $Seconds) {
        $gyroReading = $gyro.GetCurrentReading()
        $accelReading = $accel.GetCurrentReading()
        if ($null -ne $gyroReading -and $null -ne $accelReading) {
            if ($gyroReading.Timestamp -ne $lastGyroTimestamp) {
                $gyroSamples++
                $lastGyroTimestamp = $gyroReading.Timestamp
            }
            if ($accelReading.Timestamp -ne $lastAccelTimestamp) {
                $accelSamples++
                $lastAccelTimestamp = $accelReading.Timestamp
            }
            $speed = [Math]::Sqrt(
                $gyroReading.AngularVelocityX * $gyroReading.AngularVelocityX +
                $gyroReading.AngularVelocityY * $gyroReading.AngularVelocityY +
                $gyroReading.AngularVelocityZ * $gyroReading.AngularVelocityZ)
            $maximumGyroSpeed = [Math]::Max($maximumGyroSpeed, $speed)
            Write-Host ('Gyro {0,8:F2} {1,8:F2} {2,8:F2}  |  Accel {3,7:F3} {4,7:F3} {5,7:F3}' -f
                $gyroReading.AngularVelocityX, $gyroReading.AngularVelocityY, $gyroReading.AngularVelocityZ,
                $accelReading.AccelerationX, $accelReading.AccelerationY, $accelReading.AccelerationZ)
        }
        Start-Sleep -Milliseconds 250
    }
    if ($gyroSamples -lt 2 -or $accelSamples -lt 2) {
        throw "Sensors exist, but fresh readings were not received from both. Gyro updates=$gyroSamples; accelerometer updates=$accelSamples."
    }
    Write-Host "`nBoth sensors produced fresh readings." -ForegroundColor Green
    Write-Host ('Highest observed gyro speed: {0:F2} degrees/second.' -f $maximumGyroSpeed)
    if ($maximumGyroSpeed -lt 1.0) {
        Write-Host 'Little rotation was observed. Rerun while gently tilting to check changing gyro values.'
    }
} finally {
    try { $gyro.ReportInterval = $oldGyroInterval } catch { Write-Warning 'Could not restore the gyroscope report interval.' }
    try { $accel.ReportInterval = $oldAccelInterval } catch { Write-Warning 'Could not restore the accelerometer report interval.' }
}
