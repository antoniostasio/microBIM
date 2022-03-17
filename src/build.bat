@echo off
 

:: executes the command ("%~dp0\..") and sets the variable i equal to the result
if not defined PROJ_ROOT (
    for /f "delims=" %%i in ("%~dp0\..") do ( 
        set "PROJ_ROOT=%%~fi"
    )
)

IF NOT EXIST "%PROJ_ROOT%\build" (
    mkdir "%PROJ_ROOT%\build"
)

setlocal

pushd %PROJ_ROOT%\build

set path=d:\DevEnv\msys64\mingw64\include\ncurses;%path%

set ncurses_include=-Id:\DevEnv\msys64\mingw64\include\ncurses


set DefaultCompileOptions=-g -std=c++17
set Libraries=-lncurses %ncurses_include%

g++ %DefaultCompileOptions% -o main.exe %PROJ_ROOT%\src\main.cpp %Libraries%

::clang++ -fsanitize=address %DefaultCompileOptions% -o main.exe ..\src\main.cpp %Libraries%

endlocal