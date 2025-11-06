@echo off
echo ================================================
echo    HOSTEL ALLOTMENT SYSTEM - BUILD SCRIPT
echo ================================================
echo.
echo This script will compile the C++ application.
echo Make sure you have a C++ compiler installed.
echo.

echo Checking for C++ compiler...
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: C++ compiler (g++) not found!
    echo Please install a C++ compiler like MinGW, GCC, or Visual Studio.
    echo.
    pause
    exit /b 1
)

echo C++ compiler found. Starting compilation...
echo.

echo Compiling the application...
g++ -std=c++11 -Wall -Wextra -O2 -o hostel_system.exe main.cpp Student.cpp Room.cpp Admin.cpp HostelManager.cpp

if errorlevel 1 (
    echo.
    echo ================================================
    echo    COMPILATION FAILED!
    echo ================================================
    echo.
    echo Please check the error messages above.
    echo Common issues:
    echo   - Missing C++ compiler
    echo   - Syntax errors in source code
    echo   - Missing header files
    echo.
    echo Press any key to exit...
    pause >nul
    exit /b 1
) else (
    echo.
    echo ================================================
    echo    COMPILATION SUCCESSFUL!
    echo ================================================
    echo.
    echo Executable created: hostel_system.exe
    echo.
    echo To run the application:
    echo   1. Double-click hostel_system.exe, OR
    echo   2. Run run.bat, OR
    echo   3. Type: hostel_system.exe
    echo.
    echo Default admin login:
    echo   Username: admin
    echo   Password: admin123
    echo.
    echo Press any key to continue...
    pause >nul
)


