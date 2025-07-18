@echo off
echo MasterMind Trader - Python Version
echo ==================================

echo Checking Python installation...
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Python not found! Please install Python 3.7 or higher.
    echo Download from: https://www.python.org/downloads/
    pause
    exit /b 1
)

echo Python found!
echo.

echo Installing dependencies...
pip install matplotlib numpy >nul 2>&1
if %errorlevel% neq 0 (
    echo Warning: Could not install some dependencies. The application may not work correctly.
    echo Try running: pip install -r requirements.txt
    echo.
)

echo Starting MasterMind Trader...
echo.
python mastermind_trader.py

pause 