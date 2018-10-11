cmake_minimum_required(VERSION 3.12.1)

project(pdbClient)
set(CMAKE_CXX_STANDARD 11)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find files
file(GLOB SOURCES "src/*.cpp")
file(GLOB UI_FILES "src/*.ui")

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)


add_executable(pdbClient ${SOURCES} ${UI_FILES})

target_include_directories(pdbClient PUBLIC "src/")

target_link_libraries(pdbClient Qt5::Core Qt5::Widgets)