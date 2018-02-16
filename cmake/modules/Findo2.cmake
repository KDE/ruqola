# Cmake macro to detect o2 libraries
#
# This module defines
#  o2_FOUND          - True if libo2 is detected.
#  o2_INCLUDE_DIRS   - Path to libo2 header files.
#  o2_LIBRARIES      - Libraries to link against to use libo2.
#  o2_VERSION_STRING - e.g. "2.4.14"
#  o2_VERSION_MAJOR  - e.g. "2"
#  o2_VERSION_MINOR  - e.g. "4"
#  o2_VERSION_PATCH  - e.g. "14"
#
#


set(o2_FIND_REQUIRED ${o2_FIND_REQUIRED})

find_path(o2_INCLUDE_DIRS o2/o2.h)

set(o2_NAMES      ${o2_NAMES}      o2      libo2)

find_library(o2_LIBRARY
        NAMES ${o2_NAMES} 
        ) 


# Detect libo2 version

find_package(PkgConfig)
pkg_check_modules(PC_o2 QUIET libo2)

if(PC_o2_FOUND)

    set(o2_VERSION_STRING "${PC_o2_VERSION}")

endif()

# handle the QUIETLY and REQUIRED arguments and set o2_FOUND to TRUE if
# all listed variables are TRUE

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(o2 DEFAULT_MSG
                                  o2_LIBRARY
                                  o2_INCLUDE_DIRS)

if(o2_FOUND)
    set(o2_INCLUDE_DIRS ${o2_INCLUDE_DIRS}/o2)
    set(o2_LIBRARIES    ${o2_LIBRARY} )
endif()

message(STATUS "libo2 found    : ${o2_FOUND}")
message(STATUS "libo2 version  : ${o2_VERSION_STRING}")
message(STATUS "libo2 includes : ${o2_INCLUDE_DIRS}")
message(STATUS "libo2 libraries: ${o2_LIBRARIES}")
