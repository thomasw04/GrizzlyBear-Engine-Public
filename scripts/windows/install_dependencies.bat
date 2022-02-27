@echo off

::Script to install libraries for the engine

pushd ..\..\

call libs\vcpkg\bootstrap-vcpkg.bat -disableMetrics

for /f "tokens=*" %%a in (packages.txt) do call :installPackage %%a

for /f "usebackq delims=" %%i in (`libs\vswhere\vswhere.exe -prerelease -latest -property installationPath`) do (
  if exist "%%i\Common7\Tools\vsdevcmd.bat" (
    call "%%i\Common7\Tools\vsdevcmd.bat" -arch=amd64 %* & cd projects\LuaJit\luajit\src & msvcbuild.bat
    popd

    echo Installed dependencies. Ready for run.
	pause
    exit /b
  )
)

rem Instance or command prompt not found
echo Visual Studio Command Prompt not found

popd
pause

:installPackage
echo Trying to install %*...
call libs\vcpkg\vcpkg.exe install %*:x64-windows-static
