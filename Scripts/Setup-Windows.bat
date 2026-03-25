::@echo off

::pushd ..
::Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua vs2022
::popd
::pause


@echo off
setlocal

pushd ..

:: Download Premake if not present
if not exist "Vendor\Binaries\Premake\Windows\premake5.exe" (
    echo Downloading Premake...
    mkdir Vendor\Binaries\Premake\Windows
    curl -L -o premake.zip https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-windows.zip
    tar -xf premake.zip -C Vendor\Binaries\Premake\Windows
    del premake.zip
)

Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua gmake2

make config=release

for /r Binaries %%f in (App.exe) do set BINARY=%%f

if defined CI (
    echo Build successful! Running smoke test...
    echo hello world | "%BINARY%" -E "hello" > result.txt
    set /p RESULT=<result.txt
    del result.txt
    if "%RESULT%"=="hello world" (
        echo Smoke test passed!
    ) else (
        echo Smoke test failed!
        exit /b 1
    )
) else (
    copy "%BINARY%" "C:\Windows\System32\grape.exe"
    echo Installed! Try: grape -E ^<regex^> ^<file.txt^>
)

popd
pause
