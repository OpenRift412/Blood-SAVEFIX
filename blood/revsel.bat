@echo off
if "%MSG%" == "" goto error
set CHOICE=
cls
echo [1] Shareware 1.11 (23 September 1997)
echo [2] Registered 1.11 (23 September 1997)
echo [3] Registered 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
echo [4] Plasma Pak 1.11 (23 September 1997)
echo [5] Plasma Pak 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
echo [6] Plasma Pak 1.21 (One Unit Whole Blood)
echo.
echo [0] Cancel and quit
echo.
echo * Watcom 10.6 should be used for a more accurate code generation,
echo along with TASM 3.1.
echo.
echo %MSG%
set MSG=
choice /S /C:1234560 /N
echo.

if ERRORLEVEL 7 goto end
if ERRORLEVEL 6 goto BP121
if ERRORLEVEL 5 goto BP120
if ERRORLEVEL 4 goto BP111A
if ERRORLEVEL 3 goto BR120
if ERRORLEVEL 2 goto BR111A
if ERRORLEVEL 1 goto BS111A

:BS111A
set CHOICE=BS111A
goto end
:BR111A
set CHOICE=BR111A
goto end
:BR120
set CHOICE=BR120
goto end
:BP111A
set CHOICE=BP111A
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