@echo off
if "%MSG%" == "" goto error
set CHOICE=
cls
echo [1] QTools 1.11 (19 August 1997)
echo [2] QTools 1.11 (23 September 1997)
echo.
echo [0] Cancel and quit
echo.
echo * Watcom 10.6 should be used for a more accurate code generation,
echo along with TASM 3.1.
echo.
echo %MSG%
set MSG=
choice /S /C:120 /N
echo.

if ERRORLEVEL 3 goto end
if ERRORLEVEL 2 goto QT111A
if ERRORLEVEL 1 goto QT111

:QT111
set CHOICE=QT111
goto end
:QT111A
set CHOICE=QT111A
goto end

:error
echo This script shouldn't be run independently

:end