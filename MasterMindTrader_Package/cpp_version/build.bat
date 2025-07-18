@echo off
echo MasterMind Trader - Build Script
echo ================================

:: Check if build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

:: Navigate to build directory
cd build

:: Configure with CMake
echo Configuring project with CMake...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

:: Check if CMake configuration was successful
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

:: Build the project
echo Building MasterMind Trader...
cmake --build . --config Release

:: Check if build was successful
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build completed successfully!
echo You can now run MasterMindTrader.exe
pause 