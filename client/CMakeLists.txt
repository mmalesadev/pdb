cmake_minimum_required(VERSION 3.12.1)

project(pdbClient)
set(CMAKE_CXX_STANDARD 11)

# Instruct CMake to run moc automatically when needed (needed for Qt)
set(CMAKE_AUTOMOC ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Platform specific variables
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_SYSTEM "win64")
    endif()
    set(GUI_FLAG WIN32)
elseif(UNIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_SYSTEM "linux64")
    endif()
endif()

# Find the QtWidgets library
if (WIN32)
    if (NOT DEFINED ENV{QTDIR})
        message(FATAL_ERROR "QTDIR enviroment variable not set!")
    endif()
    message(STATUS "Found QTDIR: $ENV{QTDIR}")
    set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()
find_package(Qt5Widgets CONFIG REQUIRED)

# Create main target
add_executable(pdbClient ${GUI_FLAG} "")
target_sources(pdbClient
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/mainwindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/mainwindow.h"
        "${CMAKE_CURRENT_LIST_DIR}/src/mainwindow.ui"
)
target_include_directories(pdbClient PUBLIC "src/")
target_link_libraries(pdbClient Qt5::Core Qt5::Widgets)

# Create a post-build DLL copy command
add_custom_command(TARGET pdbClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/libs/${TARGET_SYSTEM}/$<CONFIGURATION>/"
    $<TARGET_FILE_DIR:pdbClient>)