@echo off

set "SCRIPT_DIR=%~dp0"

for /f "tokens=*" %%i in ('"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath') do set VS_PATH=%%i

if defined VS_PATH (
    call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x64
) else (
    echo Warning: vswhere did not find Visual Studio installation. Continuing without vcvarsall.
)

set "PATH=C:\Program Files\LLVM\bin;%PATH%"

if not exist bin mkdir bin

rem Generate makefiles with premake5 (gmake)
if exist "%SCRIPT_DIR%premake5.lua" (
    where premake5.exe >nul 2>nul
    if %ERRORLEVEL%==0 (
        premake5 gmake || (echo premake failed && goto :skip_make)
    ) else (
        if exist "C:\inc\premake-5.0.0-beta8-windows\premake5.exe" (
            "C:\inc\premake-5.0.0-beta8-windows\premake5.exe" gmake || (echo premake failed && goto :skip_make)
        ) else (
            echo premake not found. Please install premake or add it to PATH.
            goto :skip_make
        )
    )
) else (
    echo premake5.lua not found — skipping generation.
)

if exist build (
    pushd build
    make config=debug
    popd
) else (
    echo Build directory not found — premake likely failed to generate files.
)

:skip_make

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
