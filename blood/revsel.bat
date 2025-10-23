@echo off
if "%MSG%" == "" goto error
set CHOICE=
cls
echo [1] PLASMA PAK 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
echo [2] One Unit Whole Blood 1.21
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
if ERRORLEVEL 2 goto BL121
if ERRORLEVEL 1 goto BL120

:BL120
set CHOICE=BL120
goto end
:BL121
set CHOICE=BL121
goto end

:error
echo This script shouldn't be run independently

:end