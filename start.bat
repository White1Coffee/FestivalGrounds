@echo off
setlocal
cd /d "%~dp0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0setup.ps1" -Build
if errorlevel 1 (
    echo.
    echo Setup of build mislukt. Lees de melding hierboven.
    pause
    exit /b 1
)

set "GAME_EXE=%~dp0build\Release\festival_grounds.exe"
if not exist "%GAME_EXE%" set "GAME_EXE=%~dp0build\festival_grounds.exe"
if not exist "%GAME_EXE%" (
    echo Kan festival_grounds.exe niet vinden in de build-map.
    pause
    exit /b 1
)

"%GAME_EXE%"
if errorlevel 1 (
    echo.
    echo Het spel is afgesloten met een fout.
    pause
    exit /b 1
)
endlocal
