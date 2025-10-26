@echo off
if "%MSG%" == "" goto error
set CHOICE=
cls
echo [1] Shareware  1.11 (23 September 1997)
echo [2] Registered 1.11 (19 August 1997)
echo [3] Registered 1.11 (23 September 1997)
echo [4] Registered 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
echo [5] Plasma Pak 1.11 (19 August 1997)
echo [6] Plasma Pak 1.11 (23 September 1997)
echo [7] Plasma Pak 1.20 (BLOOD.EXE from 3DFX patch, NOT 3DFX.EXE)
echo [8] Plasma Pak 1.21 (One Unit Whole Blood)
echo.
echo [0] Cancel and quit
echo.
echo * Watcom 10.6 should be used for a more accurate code generation,
echo along with TASM 3.1.
echo.
echo %MSG%
set MSG=
choice /S /C:123456780 /N
echo.

if ERRORLEVEL 9 goto end
if ERRORLEVEL 8 goto BP121
if ERRORLEVEL 7 goto BP120
if ERRORLEVEL 6 goto BP111A
if ERRORLEVEL 5 goto BP111
if ERRORLEVEL 4 goto BR120
if ERRORLEVEL 3 goto BR111A
if ERRORLEVEL 2 goto BR111
if ERRORLEVEL 1 goto BS111A

:BS111A
set CHOICE=BS111A
set QTCHOICE=QT111A
goto end
:BR111
set CHOICE=BR111
set QTCHOICE=QT111
goto end
:BR111A
set CHOICE=BR111A
set QTCHOICE=QT111A
goto end
:BR120
set CHOICE=BR120
set QTCHOICE=QT111A
goto end
:BP111
set CHOICE=BP111
set QTCHOICE=QT111
goto end
:BP111A
set CHOICE=BP111A
set QTCHOICE=QT111A
goto end
:BP120
set CHOICE=BP120
set QTCHOICE=QT111A
goto end
:BP121
set CHOICE=BP121
set QTCHOICE=QT111A
goto end

:error
echo This script shouldn't be run independently

:end