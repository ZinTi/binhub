@echo off
setlocal enabledelayedexpansion

:: Compilation configuration
set CC=g++
set CFLAGS=-std=c++17 -Wall -Os
set OUTPUT=digest.exe
set SOURCES=main.cpp HashUtils.cpp

:: OpenSSL paths (consistent with CMake configuration)
set OPENSSL_ROOT=A:/build/openssl-win64-mingw
set OPENSSL_INCLUDE=-I"%OPENSSL_ROOT%/include"
set OPENSSL_LIB_DIR=-L"%OPENSSL_ROOT%/lib64"
set OPENSSL_LIBS=-lssl -lcrypto

:: Windows system libraries
set WIN_LIBS=-lws2_32 -luser32 -ladvapi32 -lcrypt32

:: Build full compile command
set COMPILE_CMD=%CC% %CFLAGS% %OPENSSL_INCLUDE% %SOURCES% -o %OUTPUT% %OPENSSL_LIB_DIR% %OPENSSL_LIBS% %WIN_LIBS% -static

:: Execute compilation
echo Compiling project...
%COMPILE_CMD%

:: Check compilation result
if errorlevel 1 (
    echo Compilation failed!
    exit /b 1
) else (
    echo Successfully built executable: %OUTPUT%
)

endlocal
