@echo off
setlocal enabledelayedexpansion

REM ===========================
REM Check for environment variable or fallback to hardcoded default
REM ===========================
if defined QT_ROOT (
    echo [INFO] Using Qt path from environment variable: %QT_ROOT%
) else if "%~1"=="" (
    echo [ERROR] QT_ROOT is not set and no argument provided.
    echo Please run: set QT_ROOT=Your/Qt/Path before executing this script.
    pause
    exit /b 1
) else (
    set "QT_ROOT=%~1"
    echo [INFO] Using Qt path from argument: %QT_ROOT%
)

set "QT_BIN_DIR=%QT_ROOT%\bin"
set "QT_CMAKE_DIR=%QT_ROOT%\lib\cmake\Qt5"

REM ===========================
REM Add Qt bin directory to PATH
REM ===========================
set PATH=%QT_BIN_DIR%;%PATH%

REM ===========================
REM Switch to the project root directory
REM ===========================
cd /d %~dp0

REM ===========================
REM Run CMake to generate build files
REM ===========================
echo === [CMake] Generating build files ===
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DQt5_DIR="%QT_CMAKE_DIR%"

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

REM ===========================
REM Build the project
REM ===========================
echo === [CMake] Building configuration ===
cmake --build build --config Release --parallel

if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

REM ===========================
REM Run the compiled application
REM ===========================
set "EXE_PATH=%~dp0bin\Release\TacViewLike.exe"

if exist "!EXE_PATH!" (
    echo === Run TacViewLike.exe ===
    cd /d "%~dp0bin\Release"
    TacViewLike.exe
) else (
    echo [ERROR] Executable not found: !EXE_PATH!
    pause
    exit /b 1
)
