@echo off
echo ================================================================
echo MasterMind Trader - New PC Setup Script
echo ================================================================
echo.

echo Step 1: Checking Python installation...
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Python is not installed or not in PATH!
    echo.
    echo Please install Python from: https://www.python.org/downloads/
    echo Make sure to check "Add Python to PATH" during installation
    echo.
    echo After installing Python, run this script again.
    pause
    exit /b 1
)

echo [OK] Python is installed
python --version

echo.
echo Step 2: Installing required packages...
echo Installing matplotlib...
pip install matplotlib --quiet
if %errorlevel% neq 0 (
    echo [WARNING] Failed to install matplotlib
)

echo Installing numpy...
pip install numpy --quiet
if %errorlevel% neq 0 (
    echo [WARNING] Failed to install numpy
)

echo Installing all requirements...
pip install -r requirements.txt --quiet
if %errorlevel% neq 0 (
    echo [WARNING] Some packages may not have installed correctly
)

echo.
echo Step 3: Testing tkinter (GUI library)...
python -c "import tkinter; print('tkinter is working')" >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] tkinter is not working. You may need to reinstall Python with tkinter support.
    echo Try: pip install tk
) else (
    echo [OK] tkinter is working
)

echo.
echo Step 4: Testing all dependencies...
python -c "import matplotlib; import numpy; import tkinter; print('All dependencies OK')" >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Some dependencies are missing. Please install them manually:
    echo pip install matplotlib numpy
    pause
    exit /b 1
) else (
    echo [OK] All dependencies are installed
)

echo.
echo Step 5: Starting MasterMind Trader...
echo ================================================================
echo Starting the application...
echo You should see a trading interface window opening.
echo ================================================================
echo.

python mastermind_trader.py

echo.
echo ================================================================
echo If you didn't see the interface:
echo 1. Check if Python is properly installed
echo 2. Try running: python mastermind_trader.py
echo 3. Check for error messages above
echo ================================================================
pause 