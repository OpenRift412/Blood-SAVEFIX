@echo off
if "%MSG%" == "" goto error
set CHOICE=
cls
echo [1] Registered 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
echo [2] Plasma Pak 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
echo [3] One Unit Whole Blood 1.21
echo.
echo [0] Cancel and quit
echo.
echo * Watcom 10.6 should be used for a more accurate code generation,
echo along with TASM 3.1.
echo.
echo %MSG%
set MSG=
choice /S /C:1230 /N
echo.

if ERRORLEVEL 4 goto end
if ERRORLEVEL 3 goto BP121
if ERRORLEVEL 2 goto BP120
if ERRORLEVEL 1 goto BR120

:BR120
set CHOICE=BR120
goto end
:BP120
set CHOICE=BP120
goto end
:BP121
set CHOICE=BP121
goto end

:error
echo This script shouldn't be run independently

:end