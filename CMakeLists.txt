cmake_minimum_required(VERSION 3.10)

project(QtFileDialogExample VERSION 1.0)

# Tell CMake where to find Qt
set(CMAKE_AUTOMOC ON)


find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Create the executable
add_executable(QtFileDialogExample MACOSX_BUNDLE main.cpp)

# Link the Qt5 Widgets library
target_link_libraries(QtFileDialogExample Qt6::Widgets)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)