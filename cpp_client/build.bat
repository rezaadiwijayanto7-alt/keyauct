@echo off
title Build GitHub KeyAuth C++ Client
echo ========================================================
echo   MEMBANGUN CLIENT C++ (GitHub KeyAuth Online)
echo ========================================================
echo.

where g++ >nul 2>nul
if %errorlevel% equ 0 (
    echo [*] Menggunakan MinGW g++ compiler...
    g++ main.cpp -o main.exe -lwininet -static-libgcc -static-libstdc++
    if %errorlevel% equ 0 (
        echo [OK] Selesai compile: main.exe
        echo.
        main.exe
        goto done
    )
)

where cl >nul 2>nul
if %errorlevel% equ 0 (
    echo [*] Menggunakan MSVC cl.exe...
    cl /EHsc main.cpp /Fe:main.exe /link wininet.lib
    if %errorlevel% equ 0 (
        echo [OK] Selesai compile: main.exe
        echo.
        main.exe
        goto done
    )
)

echo [INFO] Compiler g++ / cl tidak ditemukan di PATH.
echo Jika memakai MinGW di CMD:
echo   g++ main.cpp -o main.exe -lwininet
echo.

:done
pause
