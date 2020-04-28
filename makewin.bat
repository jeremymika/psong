@echo off
cmake . -B build.win
cmake --build build.win

echo Executable should be in build.win\bin\Debug
