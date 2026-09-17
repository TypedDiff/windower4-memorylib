@echo off
setlocal enabledelayedexpansion

set PROJECT_FILE=

REM --- Locate MSBuild via vswhere (works for any installed VS edition) ---
set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist %VSWHERE% (
    echo ERROR: vswhere.exe not found. Is Visual Studio installed?
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`%VSWHERE% -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
    set MSBUILD_PATH=%%i
)

if not defined MSBUILD_PATH (
    echo ERROR: MSBuild.exe not found via vswhere.
    exit /b 1
)

echo Using MSBuild: %MSBUILD_PATH%
echo Building: MemoryLib.sln  [Configuration=Release, Platform=x86]
echo.

"%MSBUILD_PATH%" "MemoryLib.sln" /p:Configuration=Release /p:Platform=x86 /m

if errorlevel 1 (
    echo.
    echo BUILD FAILED.
    exit /b 1
) else (
    echo.
    echo BUILD SUCCEEDED.
)

endlocal