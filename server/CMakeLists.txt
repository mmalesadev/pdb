cmake_minimum_required(VERSION 3.12.1)

project(pdb)
set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "src/*.cpp")
add_executable(pdb ${SOURCES})

target_include_directories(pdb PUBLIC "src/")
