@echo off
echo ================================================================
echo MasterMind Trader - COMPLETE TESTING GUIDE
echo ================================================================
echo.
echo This script will perform a comprehensive test of both C++ and Python
echo versions of the MasterMind Trader and generate a client report.
echo.
pause

echo Step 1: Creating test results directory...
if not exist "test_results" mkdir test_results
cd test_results

echo.
echo ================================================================
echo PHASE 1: C++ VERSION TESTING
echo ================================================================
echo.
pause

echo Running automated C++ tests...
call ..\tests\automated_test_cpp.bat

echo.
echo ================================================================
echo PHASE 2: PYTHON VERSION TESTING  
echo ================================================================
echo.
pause

echo Running automated Python tests...
call ..\tests\automated_test_python.bat

echo.
echo ================================================================
echo PHASE 3: MANUAL TESTING CHECKLIST
echo ================================================================
echo.
echo Opening the manual testing checklist...
echo Please fill out the checklist while testing both versions.
pause
start notepad ..\tests\system_verification_checklist.md

echo.
echo ================================================================
echo PHASE 4: SCREENSHOT COLLECTION
echo ================================================================
echo.
echo Please take screenshots of:
echo 1. C++ version main window
echo 2. C++ version trading dashboard
echo 3. C++ version charts and logs
echo 4. Python version main window  
echo 5. Python version dashboard
echo 6. Python version running simulation
echo.
echo Save screenshots in the test_results folder.
echo.
pause

echo.
echo ================================================================
echo PHASE 5: CLIENT REPORT PREPARATION
echo ================================================================
echo.
echo Opening the client report template...
echo Please fill in your test results and findings.
pause
copy ..\tests\CLIENT_REPORT_TEMPLATE.md client_report.md
start notepad client_report.md

echo.
echo ================================================================
echo TESTING COMPLETE!
echo ================================================================
echo.
echo All test files are available in the test_results folder:
echo - test_results_cpp.log (C++ test results)
echo - test_results_python.log (Python test results)  
echo - system_verification_checklist.md (manual testing)
echo - client_report.md (client report template)
echo - Screenshots (add your screenshots here)
echo.
echo Next Steps:
echo 1. Complete the manual checklist
echo 2. Fill in the client report template
echo 3. Add screenshots to demonstrate functionality
echo 4. Send the complete test_results folder to your client
echo.
pause

echo Opening test results folder...
explorer .

cd ..
echo.
echo ================================================================
echo Thank you for using the MasterMind Trader Testing System!
echo ================================================================ 