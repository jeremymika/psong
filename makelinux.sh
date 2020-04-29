#!/bin/sh
cmake . -B build.linux
cmake --build build.linux

echo Executable should be in build.linux/bin
