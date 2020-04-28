#!/bin/sh
cmake . -B build.mac
cmake --build build.mac

echo Executable should be in build.mac/bin
