@echo off

for /f "tokens=*" %%i in ('"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath') do set VS_PATH=%%i

call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x64

set "PATH=C:\Program Files\LLVM\bin;%PATH%"

if not exist bin mkdir bin

C:\inc\premake-5.0.0-beta8-windows\premake5.exe gmake

cd build
make config=debug
set "SCRIPT_DIR=%~dp0"
set "SDL3_BASE=%SCRIPT_DIR%third_party\SDL3-3.4.2\lib"

if exist "%SDL3_BASE%\x64\SDL3.dll" (
    copy /Y "%SDL3_BASE%\x64\SDL3.dll" "%SCRIPT_DIR%bin\SDL3.dll" >nul
    echo Copied SDL3.dll x64 to bin
    goto :done_copy
)

if exist "%SDL3_BASE%\x86\SDL3.dll" (
    copy /Y "%SDL3_BASE%\x86\SDL3.dll" "%SCRIPT_DIR%bin\SDL3.dll" >nul
    echo Copied SDL3.dll x86 to bin
    goto :done_copy
)

if exist "%SDL3_BASE%\arm64\SDL3.dll" (
    copy /Y "%SDL3_BASE%\arm64\SDL3.dll" "%SCRIPT_DIR%bin\SDL3.dll" >nul
    echo Copied SDL3.dll arm64 to bin
    goto :done_copy
)

echo Warning: SDL3.dll not found under "%SDL3_BASE%". Please ensure SDL is available.

:done_copy
