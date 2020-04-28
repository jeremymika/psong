# Created: Apr 27, 2020
# Author:  Jeremy Mika

cmake_minimum_required(VERSION 3.5.1)

project(psong_root VERSION 0.1.0)

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


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

ExternalProject_Add(thirdparty
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/thirdparty"
  INSTALL_COMMAND ""
  PREFIX ""
)

ExternalProject_Add(psong
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/psong"
  INSTALL_COMMAND ""
  PREFIX ""
  
  DEPENDS thirdparty
)