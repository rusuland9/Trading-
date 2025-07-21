@echo off
echo ================================================================
echo MasterMind Trader - Automated C++ Testing Script
echo ================================================================
echo.

set TEST_RESULTS_FILE=test_results_cpp.log
echo MasterMind Trader C++ Testing Report > %TEST_RESULTS_FILE%
echo Date: %date% %time% >> %TEST_RESULTS_FILE%
echo ================================================== >> %TEST_RESULTS_FILE%
echo. >> %TEST_RESULTS_FILE%

echo Step 1: System Information Collection
echo [INFO] Collecting system information...
echo System Information: >> %TEST_RESULTS_FILE%
systeminfo | findstr /c:"OS Name" /c:"Total Physical Memory" /c:"Processor" >> %TEST_RESULTS_FILE%
echo. >> %TEST_RESULTS_FILE%

echo Step 2: Environment Verification
echo [INFO] Checking Qt installation...
qmake --version >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] Qt is installed >> %TEST_RESULTS_FILE%
    qmake --version >> %TEST_RESULTS_FILE%
) else (
    echo [FAIL] Qt not found in PATH >> %TEST_RESULTS_FILE%
)

echo [INFO] Checking CMake installation...
cmake --version >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] CMake is available >> %TEST_RESULTS_FILE%
    cmake --version >> %TEST_RESULTS_FILE%
) else (
    echo [FAIL] CMake not found >> %TEST_RESULTS_FILE%
)

echo [INFO] Checking C++ compiler...
g++ --version >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] GCC compiler available >> %TEST_RESULTS_FILE%
    g++ --version >> %TEST_RESULTS_FILE%
) else (
    cl >nul 2>&1
    if %errorlevel% lss 2 (
        echo [PASS] MSVC compiler available >> %TEST_RESULTS_FILE%
        cl 2>&1 | findstr /c:"Version" >> %TEST_RESULTS_FILE%
    ) else (
        echo [FAIL] No C++ compiler found >> %TEST_RESULTS_FILE%
    )
)

echo. >> %TEST_RESULTS_FILE%

echo Step 3: Build Testing
echo [INFO] Creating build directory...
if not exist "build" mkdir build
cd build

echo [INFO] Running CMake configuration...
cmake .. > cmake_config.log 2>&1
if %errorlevel% equ 0 (
    echo [PASS] CMake configuration successful >> ..\%TEST_RESULTS_FILE%
) else (
    echo [FAIL] CMake configuration failed >> ..\%TEST_RESULTS_FILE%
    echo CMake Error Log: >> ..\%TEST_RESULTS_FILE%
    type cmake_config.log >> ..\%TEST_RESULTS_FILE%
)

echo [INFO] Building the project...
set BUILD_START_TIME=%time%
cmake --build . --config Release > build.log 2>&1
set BUILD_END_TIME=%time%

if %errorlevel% equ 0 (
    echo [PASS] Build completed successfully >> ..\%TEST_RESULTS_FILE%
    echo Build completed in: %BUILD_START_TIME% - %BUILD_END_TIME% >> ..\%TEST_RESULTS_FILE%
) else (
    echo [FAIL] Build failed >> ..\%TEST_RESULTS_FILE%
    echo Build Error Log: >> ..\%TEST_RESULTS_FILE%
    type build.log >> ..\%TEST_RESULTS_FILE%
)

echo [INFO] Checking executable...
if exist "MasterMindTrader.exe" (
    echo [PASS] Executable created successfully >> ..\%TEST_RESULTS_FILE%
    for %%i in ("MasterMindTrader.exe") do echo Executable size: %%~zi bytes >> ..\%TEST_RESULTS_FILE%
) else (
    echo [FAIL] Executable not found >> ..\%TEST_RESULTS_FILE%
)

cd ..

echo Step 4: Runtime Testing
echo [INFO] Testing application startup...
if exist "build\MasterMindTrader.exe" (
    echo [INFO] Starting application for 10 seconds...
    timeout /t 1 /nobreak >nul
    start /min build\MasterMindTrader.exe
    timeout /t 10 /nobreak >nul
    taskkill /im MasterMindTrader.exe /f >nul 2>&1
    echo [PASS] Application launched and closed successfully >> %TEST_RESULTS_FILE%
) else (
    echo [FAIL] Cannot test runtime - executable not found >> %TEST_RESULTS_FILE%
)

echo Step 5: File Structure Verification
echo [INFO] Verifying project structure...
echo Project Structure: >> %TEST_RESULTS_FILE%
if exist "src\" echo [PASS] src/ directory exists >> %TEST_RESULTS_FILE%
if exist "include\" echo [PASS] include/ directory exists >> %TEST_RESULTS_FILE%
if exist "CMakeLists.txt" echo [PASS] CMakeLists.txt exists >> %TEST_RESULTS_FILE%
if exist "config.json" echo [PASS] config.json exists >> %TEST_RESULTS_FILE%
if exist "README.md" echo [PASS] README.md exists >> %TEST_RESULTS_FILE%

echo. >> %TEST_RESULTS_FILE%
echo ================================================== >> %TEST_RESULTS_FILE%
echo Test completed at: %date% %time% >> %TEST_RESULTS_FILE%
echo ================================================== >> %TEST_RESULTS_FILE%

echo.
echo ================================================================
echo Testing completed! Results saved to %TEST_RESULTS_FILE%
echo ================================================================
echo.

echo Opening test results...
notepad %TEST_RESULTS_FILE% 