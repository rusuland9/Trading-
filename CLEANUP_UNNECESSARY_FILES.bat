@echo off
echo ================================================================
echo MasterMind Trader - Project Cleanup Script
echo ================================================================
echo.
echo This script will identify files that are NOT essential for the core project.
echo.

echo ================================================================
echo UNNECESSARY FILES THAT CAN BE REMOVED:
echo ================================================================
echo.

echo BUILD FILES (can be regenerated):
if exist "build\" echo [REMOVABLE] build/ - Build output directory
if exist "CMakeFiles\" echo [REMOVABLE] CMakeFiles/ - CMake temporary files  
if exist "CMakeCache.txt" echo [REMOVABLE] CMakeCache.txt - CMake cache
if exist "*.vcxproj*" echo [REMOVABLE] *.vcxproj* - Visual Studio project files
if exist "Makefile" echo [REMOVABLE] Makefile - Generated makefile

echo.
echo DEVELOPMENT FILES (optional):
if exist ".git\" echo [OPTIONAL] .git/ - Git version control (keep if using Git)
if exist ".gitignore" echo [OPTIONAL] .gitignore - Git ignore rules
if exist ".github\" echo [OPTIONAL] .github/ - GitHub Actions (for CI/CD)

echo.
echo TESTING FILES (optional for deployment):
if exist "tests\system_verification_checklist.md" echo [OPTIONAL] tests\system_verification_checklist.md
if exist "tests\CLIENT_REPORT_TEMPLATE.md" echo [OPTIONAL] tests\CLIENT_REPORT_TEMPLATE.md  
if exist "COMPLETE_TESTING_GUIDE.bat" echo [OPTIONAL] COMPLETE_TESTING_GUIDE.bat
if exist "CLEANUP_UNNECESSARY_FILES.bat" echo [OPTIONAL] CLEANUP_UNNECESSARY_FILES.bat (this file)

echo.
echo LOGS AND TEMPORARY FILES:
if exist "*.log" echo [REMOVABLE] *.log - Log files
if exist "test_results\" echo [REMOVABLE] test_results/ - Testing output
if exist "__pycache__\" echo [REMOVABLE] __pycache__/ - Python cache
if exist "*.pyc" echo [REMOVABLE] *.pyc - Python compiled files

echo.
echo ================================================================
echo CORE ESSENTIAL FILES (NEVER REMOVE):
echo ================================================================
if exist "src\" echo [ESSENTIAL] src/ - C++ source code
if exist "include\" echo [ESSENTIAL] include/ - C++ headers
if exist "python_version\" echo [ESSENTIAL] python_version/ - Python implementation
if exist "requirements\" echo [ESSENTIAL] requirements/ - Project requirements
if exist "CMakeLists.txt" echo [ESSENTIAL] CMakeLists.txt - Build configuration
if exist "config.json" echo [ESSENTIAL] config.json - Application configuration
if exist "README.md" echo [ESSENTIAL] README.md - Main documentation
if exist "SETUP.md" echo [ESSENTIAL] SETUP.md - Setup instructions
if exist "PROJECT_SUMMARY.md" echo [ESSENTIAL] PROJECT_SUMMARY.md - Technical summary
if exist "package.json" echo [ESSENTIAL] package.json - Project metadata

echo.
echo ================================================================
echo CLEANUP OPTIONS:
echo ================================================================
echo.
echo [1] Remove build files only (safe cleanup)
echo [2] Remove all unnecessary files (aggressive cleanup)  
echo [3] Show file sizes (analyze disk usage)
echo [4] Exit without changes
echo.
set /p choice="Select option (1-4): "

if "%choice%"=="1" goto cleanup_build
if "%choice%"=="2" goto cleanup_all
if "%choice%"=="3" goto show_sizes
if "%choice%"=="4" goto end

:cleanup_build
echo.
echo Removing build files...
if exist "build\" rmdir /s /q build
if exist "CMakeFiles\" rmdir /s /q CMakeFiles
if exist "CMakeCache.txt" del CMakeCache.txt
if exist "*.log" del *.log
if exist "test_results\" rmdir /s /q test_results
if exist "__pycache__\" rmdir /s /q __pycache__
echo [DONE] Build files removed.
goto end

:cleanup_all  
echo.
echo WARNING: This will remove testing templates and CI files.
set /p confirm="Are you sure? (y/n): "
if not "%confirm%"=="y" goto end
echo Removing unnecessary files...
call :cleanup_build
if exist "tests\system_verification_checklist.md" del "tests\system_verification_checklist.md"
if exist "tests\CLIENT_REPORT_TEMPLATE.md" del "tests\CLIENT_REPORT_TEMPLATE.md"
if exist "COMPLETE_TESTING_GUIDE.bat" del "COMPLETE_TESTING_GUIDE.bat"
if exist ".github\" rmdir /s /q .github
echo [DONE] Aggressive cleanup completed.
goto end

:show_sizes
echo.
echo FILE SIZES ANALYSIS:
echo ================================================================
for /f %%i in ('dir /s /-c ^| find "File(s)"') do echo Total project size: %%i
if exist "build\" for /f %%i in ('dir build /s /-c ^| find "File(s)"') do echo Build files: %%i  
if exist "src\" for /f %%i in ('dir src /s /-c ^| find "File(s)"') do echo Source code: %%i
if exist "python_version\" for /f %%i in ('dir python_version /s /-c ^| find "File(s)"') do echo Python version: %%i
goto end

:end
echo.
echo ================================================================
echo Cleanup completed!
echo ================================================================
pause 