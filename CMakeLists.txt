cmake_minimum_required(VERSION 3.30)
project(AeonRxIo VERSION 0.1.0)

set(APP ${PROJECT_NAME})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Debug
include(CTest)
enable_testing()

include(CPack)

# Source Files
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)

# link_directories(${SKIA_DIR})

# Executable
add_executable(${APP} ${source_files})

# Include directories
target_include_directories(${APP} PUBLIC src src/base)


# # ----------------------------------------------------------------------
# #    MRuby
# #
# find_package(MRuby REQUIRED)

# if (MRUBY_FOUND)
#     message(STATUS "MRuby found!")
#     target_include_directories(${APP} PRIVATE ${MRUBY_INCLUDE_DIR})
#     target_link_libraries(${APP} PRIVATE ${MRUBY_LIBRARIES})
# else()
#     message(FATAL_ERROR "MRuby not found!")
# endif()
# # ----------------------------------------------------------------------


# ----------------------------------------------------------------------
#    Ruby
#
find_package(RubyStatic REQUIRED)
target_include_directories(${APP} PRIVATE ${RubyStatic_INCLUDE_DIRS})
target_link_libraries(${APP} ${RubyStatic_LIBRARIES} -lcrypt)
target_compile_options(${APP} PRIVATE -Wno-register)
# ----------------------------------------------------------------------
