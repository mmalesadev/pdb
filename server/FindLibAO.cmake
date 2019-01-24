if(CMAKE_CROSSCOMPILING)
    find_path(LIBAO_INCLUDE_DIR
        NAMES
          ao/ao.h
        PATHS
          ${PC_LIBAO_INCLUDE_DIRS}
          NO_DEFAULT_PATH
        )

    find_library(LIBAO_LIBRARY
        NAMES
          ao
        PATHS
          ${PC_LIBAO_LIBRARY_DIRS}
          NO_DEFAULT_PATH
        )

else(CMAKE_CROSSCOMPILING)
    find_path(LIBAO_INCLUDE_DIR
        NAMES
          ao/ao.h
        PATHS
          ${PC_LIBAO_INCLUDE_DIRS}
        )

    find_library(LIBAO_LIBRARY
        NAMES
          ao
        PATHS
          ${PC_LIBAO_LIBRARY_DIRS}
        )

endif(CMAKE_CROSSCOMPILING)

set(LIBAO_LIBRARIES ${LIBAO_LIBRARY})
set(LIBAO_INCLUDE_DIRS ${LIBAO_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBAO  DEFAULT_MSG
    LIBAO_LIBRARY LIBAO_INCLUDE_DIR)
mark_as_advanced(LIBAO_LIBRARY LIBAO_INCLUDE_DIR)