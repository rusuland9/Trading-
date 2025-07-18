@echo off
echo ================================================================
echo MasterMind Trader - Package Creator
echo ================================================================
echo.

echo Creating package for transfer to new PC...
echo.

echo Step 1: Creating package directory...
if exist "MasterMindTrader_Package" rmdir /s /q "MasterMindTrader_Package"
mkdir "MasterMindTrader_Package"

echo Step 2: Copying Python version files...
mkdir "MasterMindTrader_Package\python_version"
copy "python_version\mastermind_trader.py" "MasterMindTrader_Package\python_version\"
copy "python_version\requirements.txt" "MasterMindTrader_Package\python_version\"
copy "python_version\run_trader.bat" "MasterMindTrader_Package\python_version\"
copy "python_version\setup_new_pc.bat" "MasterMindTrader_Package\python_version\"
copy "python_version\config.json" "MasterMindTrader_Package\python_version\"
copy "python_version\README.md" "MasterMindTrader_Package\python_version\"
copy "python_version\SETUP_NEW_PC.md" "MasterMindTrader_Package\python_version\"

echo Step 3: Copying C++ version files (optional)...
mkdir "MasterMindTrader_Package\cpp_version"
copy "build.bat" "MasterMindTrader_Package\cpp_version\" 2>nul
copy "build.sh" "MasterMindTrader_Package\cpp_version\" 2>nul
copy "CMakeLists.txt" "MasterMindTrader_Package\cpp_version\" 2>nul
copy "config.json" "MasterMindTrader_Package\cpp_version\" 2>nul
copy "setup_env.bat" "MasterMindTrader_Package\cpp_version\" 2>nul
copy "test_setup.bat" "MasterMindTrader_Package\cpp_version\" 2>nul
copy "SETUP.md" "MasterMindTrader_Package\cpp_version\" 2>nul
copy "README.md" "MasterMindTrader_Package\cpp_version\" 2>nul

echo Copying C++ source files...
xcopy "include" "MasterMindTrader_Package\cpp_version\include" /E /I /Q 2>nul
xcopy "src" "MasterMindTrader_Package\cpp_version\src" /E /I /Q 2>nul
xcopy "resources" "MasterMindTrader_Package\cpp_version\resources" /E /I /Q 2>nul
xcopy "ui" "MasterMindTrader_Package\cpp_version\ui" /E /I /Q 2>nul

echo Step 4: Creating documentation...
echo # MasterMind Trader - Complete Package > "MasterMindTrader_Package\README.md"
echo. >> "MasterMindTrader_Package\README.md"
echo This package contains both Python and C++ versions of MasterMind Trader. >> "MasterMindTrader_Package\README.md"
echo. >> "MasterMindTrader_Package\README.md"
echo ## Quick Start (Python Version - Recommended): >> "MasterMindTrader_Package\README.md"
echo 1. Go to python_version folder >> "MasterMindTrader_Package\README.md"
echo 2. Double-click setup_new_pc.bat >> "MasterMindTrader_Package\README.md"
echo 3. Follow the instructions >> "MasterMindTrader_Package\README.md"
echo. >> "MasterMindTrader_Package\README.md"
echo ## C++ Version (Advanced): >> "MasterMindTrader_Package\README.md"
echo 1. Go to cpp_version folder >> "MasterMindTrader_Package\README.md"
echo 2. Read SETUP.md for detailed instructions >> "MasterMindTrader_Package\README.md"
echo. >> "MasterMindTrader_Package\README.md"
echo For detailed instructions, see SETUP_NEW_PC.md in the python_version folder. >> "MasterMindTrader_Package\README.md"

echo Step 5: Creating run script for new PC...
echo @echo off > "MasterMindTrader_Package\START_HERE.bat"
echo echo Welcome to MasterMind Trader! >> "MasterMindTrader_Package\START_HERE.bat"
echo echo. >> "MasterMindTrader_Package\START_HERE.bat"
echo echo This will start the Python version (recommended). >> "MasterMindTrader_Package\START_HERE.bat"
echo echo. >> "MasterMindTrader_Package\START_HERE.bat"
echo cd python_version >> "MasterMindTrader_Package\START_HERE.bat"
echo call setup_new_pc.bat >> "MasterMindTrader_Package\START_HERE.bat"

echo.
echo ================================================================
echo SUCCESS: Package created successfully!
echo ================================================================
echo.
echo Package location: MasterMindTrader_Package\
echo.
echo To transfer to new PC:
echo 1. Copy the entire "MasterMindTrader_Package" folder
echo 2. On new PC, double-click "START_HERE.bat"
echo 3. Follow the setup instructions
echo.
echo Package contents:
echo - Python version (ready to run)
echo - C++ version (for advanced users)
echo - Complete documentation
echo - Automated setup scripts
echo.
echo ================================================================
pause 