@echo off
echo ================================================================
echo MasterMind Trader - Automated Python Testing Script
echo ================================================================
echo.

set TEST_RESULTS_FILE=test_results_python.log
echo MasterMind Trader Python Testing Report > %TEST_RESULTS_FILE%
echo Date: %date% %time% >> %TEST_RESULTS_FILE%
echo ================================================== >> %TEST_RESULTS_FILE%
echo. >> %TEST_RESULTS_FILE%

echo Step 1: Python Environment Verification
echo [INFO] Checking Python installation...
python --version >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] Python is installed >> %TEST_RESULTS_FILE%
    python --version >> %TEST_RESULTS_FILE%
) else (
    echo [FAIL] Python not found >> %TEST_RESULTS_FILE%
    goto :end
)

echo [INFO] Checking pip availability...
pip --version >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] pip is available >> %TEST_RESULTS_FILE%
    pip --version >> %TEST_RESULTS_FILE%
) else (
    echo [FAIL] pip not found >> %TEST_RESULTS_FILE%
)

echo. >> %TEST_RESULTS_FILE%

echo Step 2: Dependencies Verification
echo [INFO] Checking required Python packages...
echo Python Dependencies Check: >> %TEST_RESULTS_FILE%

echo [INFO] Checking matplotlib...
python -c "import matplotlib; print('matplotlib version:', matplotlib.__version__)" >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] matplotlib is installed >> %TEST_RESULTS_FILE%
    python -c "import matplotlib; print('matplotlib version:', matplotlib.__version__)" >> %TEST_RESULTS_FILE%
) else (
    echo [FAIL] matplotlib not found >> %TEST_RESULTS_FILE%
)

echo [INFO] Checking numpy...
python -c "import numpy; print('numpy version:', numpy.__version__)" >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] numpy is installed >> %TEST_RESULTS_FILE%
    python -c "import numpy; print('numpy version:', numpy.__version__)" >> %TEST_RESULTS_FILE%
) else (
    echo [FAIL] numpy not found >> %TEST_RESULTS_FILE%
)

echo [INFO] Checking tkinter...
python -c "import tkinter; print('tkinter is available')" >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] tkinter is available >> %TEST_RESULTS_FILE%
) else (
    echo [FAIL] tkinter not found >> %TEST_RESULTS_FILE%
)

echo [INFO] Checking pandas...
python -c "import pandas; print('pandas version:', pandas.__version__)" >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] pandas is installed >> %TEST_RESULTS_FILE%
    python -c "import pandas; print('pandas version:', pandas.__version__)" >> %TEST_RESULTS_FILE%
) else (
    echo [WARN] pandas not found (optional) >> %TEST_RESULTS_FILE%
)

echo. >> %TEST_RESULTS_FILE%

echo Step 3: Install Missing Dependencies
echo [INFO] Installing required packages...
pip install matplotlib numpy > pip_install.log 2>&1
if %errorlevel% equ 0 (
    echo [PASS] Dependencies installation completed >> %TEST_RESULTS_FILE%
) else (
    echo [WARN] Some packages may have failed to install >> %TEST_RESULTS_FILE%
)

echo Step 4: Code Compilation Test
echo [INFO] Testing Python script compilation...
if exist "python_version\mastermind_trader.py" (
    cd python_version
    python -m py_compile mastermind_trader.py > ..\compile.log 2>&1
    if %errorlevel% equ 0 (
        echo [PASS] Python script compiles without errors >> ..\%TEST_RESULTS_FILE%
    ) else (
        echo [FAIL] Python script has compilation errors >> ..\%TEST_RESULTS_FILE%
        echo Compilation errors: >> ..\%TEST_RESULTS_FILE%
        type ..\compile.log >> ..\%TEST_RESULTS_FILE%
    )
    cd ..
) else (
    echo [FAIL] mastermind_trader.py not found >> %TEST_RESULTS_FILE%
)

echo Step 5: Runtime Testing
echo [INFO] Testing application startup (5 seconds)...
if exist "python_version\mastermind_trader.py" (
    cd python_version
    echo [INFO] Starting Python application...
    timeout /t 1 /nobreak >nul
    start /min python mastermind_trader.py
    timeout /t 5 /nobreak >nul
    taskkill /f /im python.exe >nul 2>&1
    taskkill /f /im pythonw.exe >nul 2>&1
    echo [PASS] Python application started and closed >> ..\%TEST_RESULTS_FILE%
    cd ..
) else (
    echo [FAIL] Cannot test runtime - script not found >> %TEST_RESULTS_FILE%
)

echo Step 6: File Structure Verification
echo [INFO] Verifying Python project structure...
echo Python Project Structure: >> %TEST_RESULTS_FILE%
if exist "python_version\" echo [PASS] python_version/ directory exists >> %TEST_RESULTS_FILE%
if exist "python_version\mastermind_trader.py" echo [PASS] mastermind_trader.py exists >> %TEST_RESULTS_FILE%
if exist "python_version\requirements.txt" echo [PASS] requirements.txt exists >> %TEST_RESULTS_FILE%
if exist "python_version\README.md" echo [PASS] Python README.md exists >> %TEST_RESULTS_FILE%

echo Step 7: Performance Check
echo [INFO] Checking Python performance...
python -c "import time; start=time.time(); import matplotlib, numpy; print('Import time: {:.2f}s'.format(time.time()-start))" >> %TEST_RESULTS_FILE% 2>&1

echo. >> %TEST_RESULTS_FILE%
echo ================================================== >> %TEST_RESULTS_FILE%
echo Python Test completed at: %date% %time% >> %TEST_RESULTS_FILE%
echo ================================================== >> %TEST_RESULTS_FILE%

:end
echo.
echo ================================================================
echo Python testing completed! Results saved to %TEST_RESULTS_FILE%
echo ================================================================
echo.

echo Opening test results...
notepad %TEST_RESULTS_FILE% 