#!/bin/sh
emcmake cmake . -B build.emscripten
emmake make -C build.emscripten

echo Executable should be in build.emscripten/bin
