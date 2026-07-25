cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ShowcaseKit VERSION 1.0.0)

if (NOT ANDROID)
    message(FATAL_ERROR "ShowcaseKit only targets Android32/Android64. Configure with the Android NDK toolchain (-DCMAKE_TOOLCHAIN_FILE=.../android.toolchain.cmake).")
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Import the GEODE_SDK environment variable to point at your Geode SDK checkout")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
