cmake_minimum_required(VERSION 3.30)
project(AeonRxIo VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)


# Source Files
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)

# link_directories(${SKIA_DIR})

# Executable
add_executable(${PROJECT_NAME} ${source_files})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC src)

# Debug
include(CTest)
enable_testing()

include(CPack)