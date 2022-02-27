::This script generates project files for the windows IDE Visual Studio

@if not defined _echo echo off

::vs2019 and 2022 are the only tested versions
SET ide=vs2022


IF "%1"=="vs2022" SET ide=vs2022
IF "%1"=="vs2019" SET ide=vs2019

::It is not guranteed that the vs versions below work with the source code 
IF "%1"=="vs2017" SET ide=vs2017
IF "%1"=="vs2015" SET ide=vs2015
IF "%1"=="vs2013" SET ide=vs2013
IF "%1"=="vs2012" SET ide=vs2012
IF "%1"=="vs2010" SET ide=vs2010
IF "%1"=="vs2008" SET ide=vs2008
IF "%1"=="vs2005" SET ide=vs2005

pushd %~dp0\..\..\

echo.
echo === Starting to generate GrizzlyBear Project files ===
echo.

call libs\premake\premake5-windows.exe clean
call libs\premake\premake5-windows.exe %ide% --AppProject=%2 --AppProjectLoc=%3
popd
echo GrizzlyBear %ide% project files were generated. Ready for run.
pause
