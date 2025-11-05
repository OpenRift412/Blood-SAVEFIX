@echo off
set MSG=Please select what to build:
call REVSEL.BAT

if "%CHOICE%" == "" goto end

mkdir %CHOICE%
mkdir %CHOICE%\obj
set WATINCBAK=%INCLUDE%
set INCLUDE=%INCLUDE%;..\qtools\include;..\helix32
wmake.exe %CHOICE%\qtools.lib "appver_exedef = %CHOICE%"
set CHOICE=
set INCLUDE=%WATINCBAK%
set WATINCBAK=

:end
set CHOICE=
