@echo off

setlocal
pushd ..\build

cl -Zi ..\src\*.cpp /Fe:clmain.exe

endlocal