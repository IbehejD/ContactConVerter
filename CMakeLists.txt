cmake_minimum_required(VERSION 3.16)
project(example LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)

FetchContent_Declare(
    webview
    GIT_REPOSITORY https://github.com/webview/webview
    GIT_TAG 0.12.0)
FetchContent_MakeAvailable(webview)

add_executable(example WIN32)
target_sources(example PRIVATE main.cpp)
target_link_libraries(example PRIVATE webview::core)
target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)