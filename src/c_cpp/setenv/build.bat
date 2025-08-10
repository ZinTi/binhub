@echo off
setlocal enabledelayedexpansion

set "script_dir=%~dp0"
echo Compiling project...
gcc -o %script_dir%setenv.exe %script_dir%setenv.c -Os

:: Check compilation result
if errorlevel 1 (
    echo Compilation failed!
    exit /b 1
) else (
    echo Successfully built executable: %script_dir%setenv.exe
)

endlocal
