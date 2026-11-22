param(
    [switch]$Build,
    [string]$BuildType = "Release"
)

$ErrorActionPreference = "Stop"
$projectRoot = $PSScriptRoot
$buildDir = Join-Path $projectRoot "build"

function Fail([string]$Message) {
    Write-Host "`nSetup niet voltooid: $Message" -ForegroundColor Red
    exit 1
}

$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    Fail "CMake is niet gevonden. Installeer CMake 3.20 of nieuwer en start dit script opnieuw."
}

$configureArgs = @("-S", $projectRoot, "-B", $buildDir)
if ($env:VCPKG_ROOT) {
    $toolchain = Join-Path $env:VCPKG_ROOT "scripts\buildsystems\vcpkg.cmake"
    if (Test-Path $toolchain) {
        $configureArgs += "-DCMAKE_TOOLCHAIN_FILE=$toolchain"
        $configureArgs += "-DVCPKG_TARGET_TRIPLET=x64-windows"
    }
}

Write-Host "Festival Grounds configureren..." -ForegroundColor Cyan
& $cmake.Source @configureArgs
if ($LASTEXITCODE -ne 0) {
    Fail "CMake configuratie is mislukt. Als SDL2 niet is geïnstalleerd, controleer de internetverbinding zodat CMake de officiële SDL2-broncode kan downloaden."
}

if ($Build) {
    $runningGame = Get-Process -Name "festival_grounds" -ErrorAction SilentlyContinue
    if ($runningGame) {
        Fail "Festival Grounds draait nog (PID $($runningGame[0].Id)). Sluit het spelvenster af en start daarna start.bat opnieuw; Windows vergrendelt het uitvoerbestand tijdens het draaien."
    }

    Write-Host "Festival Grounds bouwen ($BuildType)..." -ForegroundColor Cyan
    & $cmake.Source --build $buildDir --config $BuildType
    if ($LASTEXITCODE -ne 0) { Fail "Build mislukt." }
    Write-Host "Klaar. Start het spel met .\start.bat" -ForegroundColor Green
} else {
    Write-Host "Configuratie klaar. Voer .\start.bat uit om te bouwen en te starten." -ForegroundColor Green
}
