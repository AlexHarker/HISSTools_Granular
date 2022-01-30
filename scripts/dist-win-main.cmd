
@echo off
echo.
echo start build...
echo. 

call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 store

echo.
echo compile the plugs...

msbuild "%~dp0..\HISSToolsGranular.sln" /t:HISSToolsGranular-vst2;HISSToolsGranular-vst3 /p:configuration="release" /p:platform=win32 /v:q /clp:ErrorsOnly /nologo /m || exit /B 1
msbuild "%~dp0..\HISSToolsGranular.sln" /t:HISSToolsGranular-vst2;HISSToolsGranular-vst3 /p:configuration="release" /p:platform=x64 /v:q /clp:ErrorsOnly /nologo /m || exit /B 1

echo zip and move into place...
echo.

if not exist "%~dp0..\build-win" mkdir "%~dp0..\build-win"
if not exist "%~dp0..\build-win\Windows" mkdir "%~dp0..\build-win\Windows"

"C:\Program Files\7-Zip\7z.exe" a -tzip "%~dp0..\build-win\Windows\HISSToolsGranular_VST2_32bit.zip" "%~dp0..\build-win\vst2\Win32\Release\HISSToolsGranular.dll" || exit /B 1
"C:\Program Files\7-Zip\7z.exe" a -tzip "%~dp0..\build-win\Windows\HISSToolsGranular_VST2_64bit.zip" "%~dp0..\build-win\vst2\x64\Release\HISSToolsGranular.dll" || exit /B 1
"C:\Program Files\7-Zip\7z.exe" a -tzip "%~dp0..\build-win\Windows\HISSToolsGranular_VST3_32bit.zip" "%~dp0..\build-win\vst3\Win32\Release\HISSToolsGranular.vst3" || exit /B 1
"C:\Program Files\7-Zip\7z.exe" a -tzip "%~dp0..\build-win\Windows\HISSToolsGranular_VST3_64bit.zip" "%~dp0..\build-win\HISSToolsGranular.vst3" || exit /B 1

echo.
echo BUILT SUCCESSFULLY