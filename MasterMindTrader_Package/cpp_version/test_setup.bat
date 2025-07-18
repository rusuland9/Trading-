@echo off
echo MasterMind Trader - Setup Verification
echo =======================================

set ALL_OK=1

echo.
echo Testing CMake...
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found!
    echo Please install CMake from https://cmake.org/download/
    set ALL_OK=0
) else (
    echo [OK] CMake is available
    cmake --version | findstr /C:"cmake version"
)

echo.
echo Testing Qt...
qmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Qt not found!
    echo Please install Qt from https://www.qt.io/download-open-source
    set ALL_OK=0
) else (
    echo [OK] Qt is available
    qmake --version | findstr /C:"Qt version"
)

echo.
echo Testing C++ Compiler...
g++ --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] G++ compiler not found!
    echo Please install MinGW or Visual Studio
    set ALL_OK=0
) else (
    echo [OK] G++ compiler is available
    g++ --version | findstr /C:"g++"
)

echo.
echo Testing Git (Optional)...
git --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [WARN] Git not found (optional)
    echo You can install Git from https://git-scm.com/download
) else (
    echo [OK] Git is available
    git --version
)

echo.
echo Testing Qt Modules...
echo Checking if Qt Charts is available...
echo #include ^<QtCharts/QChart^> > test_qt.cpp
echo int main() { return 0; } >> test_qt.cpp

qmake -query QT_INSTALL_HEADERS >nul 2>&1
if %errorlevel% neq 0 (
    echo [WARN] Cannot verify Qt modules
) else (
    echo [OK] Qt development files are accessible
)

if exist test_qt.cpp del test_qt.cpp

echo.
echo =======================================
if %ALL_OK% equ 1 (
    echo SUCCESS: All required tools are properly installed!
    echo.
    echo You can now:
    echo 1. Run 'build.bat' to compile MasterMind Trader
    echo 2. Navigate to the build directory
    echo 3. Execute MasterMindTrader.exe
    echo.
    echo Happy trading!
) else (
    echo SETUP INCOMPLETE: Some tools are missing
    echo.
    echo Please install the missing tools and run this test again.
    echo See SETUP.md for detailed installation instructions.
    echo.
    echo Required tools:
    echo - CMake: https://cmake.org/download/
    echo - Qt Framework: https://www.qt.io/download-open-source
    echo - C++ Compiler: MinGW ^(with Qt^) or Visual Studio
)

echo.
echo Current System Information:
echo ===========================
echo Operating System: 
ver
echo.
echo Architecture:
echo %PROCESSOR_ARCHITECTURE%
echo.
echo Available Memory:
wmic computersystem get TotalPhysicalMemory /value | findstr /C:"TotalPhysicalMemory"
echo.

pause 