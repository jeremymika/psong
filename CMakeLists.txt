# Created: Apr 27, 2020
# Author:  Jeremy Mika

cmake_minimum_required(VERSION 3.11)

project(psong VERSION 0.1.0)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
        FATAL: In-source builds are not allowed.
            Create a separate directory (e.g., 'build') at the root for build files."
    )
endif()

set(DEPENDENCY_INSTALL_DIR ${CMAKE_BINARY_DIR}/thirdparty/build)
set(DEPENDENCY_DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/thirdparty/dl)
set(DEPENDENCY_EXTRACT_DIR ${CMAKE_BINARY_DIR}/thirdparty/extracted)

set(GLEW_USE_STATIC_LIBS ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if(NOT EMSCRIPTEN)

    set(LIBUV_DEPS_DIR ${CMAKE_BINARY_DIR}/thirdparty/libuv)

    ExternalProject_Add(
        libuv
        GIT_REPOSITORY https://github.com/libuv/libuv.git
        GIT_TAG ""
        TMP_DIR ${LIBUV_DEPS_DIR}/tmp
        STAMP_DIR ${LIBUV_DEPS_DIR}/stamp
        SOURCE_DIR ${LIBUV_DEPS_DIR}/src
        BUILD_IN_SOURCE 1
        GIT_SHALLOW 1

        DOWNLOAD_DIR thirdparty/libuv
        INSTALL_COMMAND ""
        TEST_COMMAND ""
        PREFIX "thirdparty/libuv"
    )

endif()


file(GLOB SOURCES src/*.cpp src/*.c thirdparty/gl3w/src/gl3w.c)
 

#include(get-sdl2_image)
#include(get-sdl2_ttf)
include(get-sdl2)

add_executable(psong ${SOURCES})
if(EMSCRIPTEN)
    add_dependencies(psong sdl2)
else()
    add_dependencies(psong libuv sdl2)
endif()

IF (APPLE)
    FIND_LIBRARY(AUDIOTOOLBOX_LIBRARY AudioToolbox )
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(COREAUDIO_LIBRARY CoreAudio)
    FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation )
    FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo )
    FIND_LIBRARY(FORCEFEEDBACK_LIBRARY ForceFeedback )
    FIND_LIBRARY(ICONV_LIBRARY iconv)
    FIND_LIBRARY(IOKIT_LIBRARY IoKit )
    set(APPLE_LIBS 
            ${AUDIOTOOLBOX_LIBRARY}
            ${CARBON_LIBRARY}
            ${COCOA_LIBRARY}
            ${COREAUDIO_LIBRARY}
            ${COREFOUNDATION_LIBRARY}
            ${COREVIDEO_LIBRARY}
            ${FORCEFEEDBACK_LIBRARY}
            ${ICONV_LIBRARY}
            ${IOKIT_LIBRARY}
        )
ELSE()
    set(APPLE_LIBS "")
ENDIF (APPLE)


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})


set(SDL2_INCLUDE_DIR ${DEPENDENCY_INSTALL_DIR}/include)

set(INCLUDE_DIRS
    /usr/local/include
    ${LIBUV_DEPS_DIR}/src/include
    ${SDL2_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/thirdparty/gl3w/include
    src)
    
include_directories(${INCLUDE_DIRS})

if(UNIX AND NOT APPLE AND NOT EMSCRIPTEN)
    set(LINUX_LIBS m)
else()
    set(LINUX_LIBS "")
endif()

if(WIN32)
    set(LIBUV_STATIC "${LIBUV_DEPS_DIR}/src/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/uv_a.lib")
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBRARY "${DEPENDENCY_INSTALL_DIR}/lib/x64/SDL2.lib;${DEPENDENCY_INSTALL_DIR}/lib/x64/SDL2main.lib")

        add_custom_command(TARGET psong POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DEPENDENCY_INSTALL_DIR}/lib/x64/SDL2.dll"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>"
            VERBATIM)
    else ()
        set(SDL2_LIBRARY "${DEPENDENCY_INSTALL_DIR}/lib/x86/SDL2.lib;${DEPENDENCY_INSTALL_DIR}/lib/x86/SDL2main.lib")
        add_custom_command(TARGET psong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DEPENDENCY_INSTALL_DIR}/lib/x86/SDL2.dll"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>" 
            VERBATIM)
    endif ()   
else()
    if(NOT EMSCRIPTEN)
        set(LIBUV_STATIC "${LIBUV_DEPS_DIR}/src/libuv_a.a")
        set(SDL2_LIBRARY ${DEPENDENCY_INSTALL_DIR}/lib/libSDL2.a)
    else()
        set(LIBUV_STATIC "")
        set(SDL2_LIBRARY "")
        endif()

endif()


set(LINK_LIBS 
    ${LIBUV_STATIC}
    ${SDL2_LIBRARY}
#    ${OPENGL_LIBRARIES}
    ${APPLE_LIBS}
    ${LINUX_LIBS}
#    ${SDL2_IMAGE_LIBRARIES}
#    ${SDL2_TTF_LIBRARIES}
    Threads::Threads
)

message("Libs: ${LINK_LIBS} ${CMAKE_DL_LIBS}")

target_link_libraries(psong ${LINK_LIBS} ${CMAKE_DL_LIBS})

if(EMSCRIPTEN)
    message("Compiling for emscripten!") 
    set_target_properties(psong PROPERTIES LINK_FLAGS   "-s MIN_WEBGL_VERSION=2 -s USE_FREETYPE=1 -s USE_SDL=2 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 -O2 -s FULL_ES3=1  -s WASM=1 -s ASSERTIONS=1 -s GL_ASSERTIONS=1 -std=c++11  -s OFFSCREENCANVAS_SUPPORT=1")
    # set_target_properties(psong PROPERTIES LINK_FLAGS "-s USE_FREETYPE=1 -s USE_SDL=2 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 -O2 -s ASSERTIONS=1  -std=c++11 -s FULL_ES3=1 -s USE_WEBGL2=1 -s OFFSCREENCANVAS_SUPPORT=1")
endif()

if(opengl)
    target_link_libraries(psong ${OPENGL_gl_LIBRARY})
#    target_link_libraries(psong ${GLEW_LIBRARY})
endif(opengl)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
