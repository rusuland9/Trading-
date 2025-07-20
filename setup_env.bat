@echo off
echo MasterMind Trader - Environment Setup
echo =======================================

:: Try to find Qt installation automatically
set QT_FOUND=0

:: Check common Qt installation paths
if exist "C:\Qt\6.8.3\mingw_64\bin" (
    set QTDIR=C:\Qt\6.8.3\mingw_64
    set QT_FOUND=1
    echo Found Qt 6.8.3 at %QTDIR%
) else if exist "C:\Qt\6.4.0\mingw_64\bin" (
    set QTDIR=C:\Qt\6.4.0\mingw_64
    set QT_FOUND=1
    echo Found Qt 6.4.0 at %QTDIR%
) else if exist "C:\Qt\6.8.3\mingw81_64\bin" (
    set QTDIR=C:\Qt\6.8.3\mingw81_64
    set QT_FOUND=1
    echo Found Qt 6.8.3 at %QTDIR%
) else (
    echo Warning: Qt installation not found in common locations
    echo Please install Qt from https://www.qt.io/download-open-source
    echo Or manually set QTDIR environment variable
)

:: Try to find CMake
set CMAKE_FOUND=0
where cmake >nul 2>&1
if %errorlevel% equ 0 (
    set CMAKE_FOUND=1
    echo Found CMake in PATH
) else (
    :: Check Qt's CMake
    if exist "C:\Qt\Tools\CMake_64\bin\cmake.exe" (
        set PATH=C:\Qt\Tools\CMake_64\bin;%PATH%
        set CMAKE_FOUND=1
        echo Found CMake in Qt tools
    ) else (
        echo Warning: CMake not found
        echo Please install CMake from https://cmake.org/download/
    )
)

:: Try to find MinGW
set MINGW_FOUND=0
where g++ >nul 2>&1
if %errorlevel% equ 0 (
    set MINGW_FOUND=1
    echo Found G++ compiler in PATH
) else (
    :: Check Qt's MinGW
    if exist "C:\Qt\Tools\mingw1310_64\bin\g++.exe" (
        set PATH=C:\Qt\Tools\mingw1310_64\bin;%PATH%
        set MINGW_FOUND=1
        echo Found MinGW in Qt tools
    ) else if exist "C:\Qt\Tools\mingw810_64\bin\g++.exe" (
        set PATH=C:\Qt\Tools\mingw810_64\bin;%PATH%
        set MINGW_FOUND=1
        echo Found MinGW in Qt tools
    ) else (
        echo Warning: MinGW compiler not found
        echo Please install Qt with MinGW or Visual Studio
    )
)

:: Set up Qt environment if found
if %QT_FOUND% equ 1 (
    set PATH=%QTDIR%\bin;%PATH%
    set QT_PLUGIN_PATH=%QTDIR%\plugins
    echo Qt environment configured
)

echo.
echo Environment Status:
echo ===================
if %QT_FOUND% equ 1 (
    echo [OK] Qt Framework
) else (
    echo [!!] Qt Framework - MISSING
)

if %CMAKE_FOUND% equ 1 (
    echo [OK] CMake Build System
) else (
    echo [!!] CMake Build System - MISSING
)

if %MINGW_FOUND% equ 1 (
    echo [OK] C++ Compiler
) else (
    echo [!!] C++ Compiler - MISSING
)

echo.
if %QT_FOUND% equ 1 if %CMAKE_FOUND% equ 1 if %MINGW_FOUND% equ 1 (
    echo SUCCESS: All tools are available!
    echo You can now run: build.bat
) else (
    echo SETUP INCOMPLETE: Please install missing tools
    echo See SETUP.md for detailed instructions
)

echo.
echo Current PATH additions:
echo %PATH%
echo.
pause 