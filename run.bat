@echo off
set QT_BIN_DIR=D:\Software\qt5.15.10-win64-debug-release-msvc2022-shared\msvc64\bin
set PATH=%QT_BIN_DIR%;%PATH%

REM 进入可执行目录
cd /d %~dp0\bin\Release

REM 启动程序
TacViewLikeDemo.exe

pause
