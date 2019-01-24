cmake_minimum_required(VERSION 3.7.2)

project(pdbServer)
set(CMAKE_CXX_STANDARD 11)

# Platform specific variables
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_SYSTEM "win64")
    endif()
elseif(UNIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_SYSTEM "linux64")
    endif()
endif()

# Find the Boost library
find_package(Boost REQUIRED COMPONENTS system)
link_directories(${Boost_LIBRARY_DIRS})

message(STATUS "BOOST LIB DIRS: ${Boost_LIBRARY_DIRS}")

# Threads
find_package(Threads REQUIRED)

# Create main target
add_executable(pdbServer ${GUI_FLAG} "")
target_sources(pdbServer
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBServer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBServer.h"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBApp.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBApp.h"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBNetwork.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PDBNetwork.h"
        "${CMAKE_CURRENT_LIST_DIR}/src/InputManager.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/InputManager.h"
        "${CMAKE_CURRENT_LIST_DIR}/src/audiobook/PDBAudiobook.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/audiobook/PDBAudiobook.h"
)
target_include_directories(pdbServer PUBLIC "src/" "src/audiobook/" ${Boost_INCLUDE_DIR})
target_link_libraries(pdbServer Threads::Threads ${Boost_LIBRARIES})

if(WIN32)
    # Create a post-build DLL copy command
    add_custom_command(TARGET pdbServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/libs/${TARGET_SYSTEM}/$<CONFIGURATION>/"
        $<TARGET_FILE_DIR:pdbServer>)
endif()
