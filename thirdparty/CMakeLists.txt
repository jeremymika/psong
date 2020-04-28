
cmake_minimum_required(VERSION 3.5.1)

project(psong_dependencies)

set(CMAKE_MODULE_PATH       ${CMAKE_SOURCE_DIR}/../cmake)

message("module path: ${CMAKE_MODULE_PATH}")

set(DEPENDENCY_INSTALL_DIR ${CMAKE_BINARY_DIR}/thirdparty/build)
set(DEPENDENCY_DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/thirdparty/dl)
set(DEPENDENCY_EXTRACT_DIR ${CMAKE_BINARY_DIR}/thirdparty/extracted)

include(get-sdl2)
include(get-sdl2_ttf)
include(get-sdl2_image)
	
