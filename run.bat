@echo off

REM 设置 Qt 环境路径
set QT_BIN_DIR=D:\Software\qt5.15.10-win64-debug-release-msvc2022-shared\msvc64\bin
set PATH=%QT_BIN_DIR%;%PATH%

REM 回到项目根目录（假设脚本位于根目录）
cd /d %~dp0

echo === [CMake] Generating build files ===
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

echo === [CMake] Building configuration ===
cmake --build build --config Release --parallel

REM 进入可执行输出目录
cd /d %~dp0\bin\Release

echo === Run TacViewLikeDemo.exe ===
TacViewLike.exe

pause
