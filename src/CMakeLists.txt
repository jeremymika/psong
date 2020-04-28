# Created: Apr 27, 2020
# Author:  Jeremy Mika

cmake_minimum_required(VERSION 3.5.1)

project(psong VERSION 0.1.0)

include(ExternalProject)

#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
        FATAL: In-source builds are not allowed.
            Create a separate directory (e.g., 'build') at the root for build files."
    )
endif()


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

set(DEPENDENCY_INSTALL_DIR ${CMAKE_BINARY_DIR}/thirdparty/build)
set(DEPENDENCY_DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/thirdparty/dl)
set(DEPENDENCY_EXTRACT_DIR ${CMAKE_BINARY_DIR}/thirdparty/extracted)


    
file(GLOB SOURCES *.cpp *.c ${WIN32_SOURCES})
 

add_executable(psong ${SOURCES})
add_dependencies(psong libuv psong_dependencies)


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})


set(INCLUDE_DIRS
    /usr/local/include
    ${LIBUV_DEPS_DIR}/src/include
    src)
    
include_directories(${INCLUDE_DIRS})

if(WIN32)
    set(LIBUV_STATIC "${LIBUV_DEPS_DIR}/src/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/uv_a.lib")
else()
    set(LIBUV_STATIC "${LIBUV_DEPS_DIR}/src/libuv_a.a")
endif()


set(LINK_LIBS 
    ${LIBUV_STATIC}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    Threads::Threads
)


target_link_libraries(psong ${LINK_LIBS})

if(opengl)
    target_link_libraries(psong ${OPENGL_gl_LIBRARY})
    target_link_libraries(psong ${GLEW_LIBRARY})
endif(opengl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
