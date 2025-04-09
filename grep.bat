@echo off
:: grep.bat: Wrapper script for win_my_grep.exe supporting relative paths & removing trailing '\'
:: Usage: grep.bat Search_Path Search_String

if "%~1"=="" (
    echo Usage: grep.bat Search_Path Search_String
    exit /b 1
)

if "%~2"=="" (
    echo Usage: grep.bat Search_Path Search_String
    exit /b 1
)

:: Try Release and Debug paths
set "base_dir=%~dp0"
set "grep_tool_release=%base_dir%bin\Release\win_my_grep.exe"
set "grep_tool_debug=%base_dir%bin\Debug\win_my_grep.exe"

:: Choose which exists
if exist "%grep_tool_release%" (
    set "grep_tool=%grep_tool_release%"
) else if exist "%grep_tool_debug%" (
    set "grep_tool=%grep_tool_debug%"
) else (
    echo Error: win_my_grep.exe not found in bin\Release or bin\Debug!
    exit /b 1
)
:: Convert first param to absolute path
set "raw_path=%~f1"

:: Remove trailing backslash (but allow root like C:\)
if "%raw_path:~-1%"=="\" (
    if not "%raw_path:~2%"=="" (
        set "raw_path=%raw_path:~0,-1%"
    )
)

:: Call grep tool
"%grep_tool%" "%raw_path%" "%~2%"