@echo off
echo ================================================
echo    HOSTEL ALLOTMENT SYSTEM - RUN SCRIPT
echo ================================================
echo.

if exist hostel_system.exe (
    echo Starting the Hostel Allotment System...
    echo.
    echo Default admin login credentials:
    echo   Username: admin
    echo   Password: admin123
    echo.
    echo ================================================
    echo.
    hostel_system.exe
) else (
    echo.
    echo ================================================
    echo    ERROR: Executable not found!
    echo ================================================
    echo.
    echo The file 'hostel_system.exe' does not exist.
    echo.
    echo To fix this:
    echo   1. Run build.bat to compile the application
    echo   2. Make sure all source files are present
    echo   3. Check for compilation errors
    echo.
    echo Press any key to exit...
    pause >nul
)


