cmake_minimum_required(VERSION 3.20)
include(FetchContent)

project(bachelor_hofbauer)

set(CMAKE_CXX_STANDARD 20)

add_executable(sys-logger main.cpp)
add_executable(process1 process1.cpp)

set(REPROC++ ON)
FetchContent_Declare(
        reproc++
        GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
        GIT_TAG        v14.2.3
)

FetchContent_MakeAvailable(reproc++)

target_link_libraries(sys-logger reproc++)

