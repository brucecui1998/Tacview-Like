@echo off

REM 切换到项目根目录（假设clean.bat放在项目根目录）
cd /d %~dp0

REM 删除bin目录
if exist bin (
    echo === Deleting bin directory ===
    rmdir /s /q bin
    echo === bin directory deleted ===
) else (
    echo === bin directory does not exist ===
)
