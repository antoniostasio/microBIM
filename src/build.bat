@echo off

@REM if NOT DEFINED project_root (
@REM set project_root=%~dp0
    @REM project_root=%project_root%\..
@REM )
@REM 
@REM pushd "%project_root%"
@REM 
@REM 
@REM set proj_root=%~dp0
@REM pushd "%proj_root%\.."
@REM set proj_root=%cd%
@REM popd

IF NOT EXIST "..\build" (
    mkdir "..\build"
)


setlocal 

pushd ..\build

set path=d:\DevEnv\msys64\mingw64\include\ncurses;%path%

set ncurses_include=-Id:\DevEnv\msys64\mingw64\include\ncurses


set DefaultCompileOptions=-g -std=c++17
set Libraries=-lncurses %ncurses_include%

g++ %DefaultCompileOptions% -o main.exe ..\src\main.cpp %Libraries%

::clang++ -fsanitize=address %DefaultCompileOptions% -o main.exe ..\src\main.cpp %Libraries%

endlocal