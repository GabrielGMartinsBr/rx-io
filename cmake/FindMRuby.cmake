# Locate the ruby-static library and include directories

set(MRubyLibDir "./3rd/mruby")

# Find the MRuby library and headers
# This module defines:
#   MRUBY_FOUND        - set to TRUE if MRuby is found
#   MRUBY_INCLUDE_DIR  - include directory for MRuby
#   MRUBY_LIBRARIES    - libraries to link against MRuby
#   MRUBY_EXECUTABLES  - MRuby-related binaries (optional)

find_path(MRUBY_INCLUDE_DIR
    NAMES mruby.h
    PATHS
        ${MRubyLibDir}/include
        ENV PATH
    NO_DEFAULT_PATH
)

find_library(MRUBY_LIBRARIES
    NAMES mruby mruby_core
    PATHS
        ${MRubyLibDir}/build/host/lib
        ENV PATH
    NO_DEFAULT_PATH
)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MRuby REQUIRED_VARS MRUBY_INCLUDE_DIR MRUBY_LIBRARIES)

mark_as_advanced(MRUBY_INCLUDE_DIR MRUBY_LIBRARIES)
