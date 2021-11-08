cmake_minimum_required(VERSION 3.20)
include(FetchContent)

project(bachelor_hofbauer)

set(CMAKE_CXX_STANDARD 20)

add_executable(sys-logger main.cpp)
add_executable(process1 process1.cpp Client.h Client.cpp)

set(REPROC++ ON)
FetchContent_Declare(
        reproc++
        GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
        GIT_TAG        v14.2.3
)
FetchContent_MakeAvailable(reproc++)


FetchContent_Declare(asio
        GIT_REPOSITORY git@github.com:chriskohlhoff/asio.git
        GIT_TAG  asio-1-20-0
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        )
FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
endif()
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
find_package(Threads)
target_link_libraries(asio INTERFACE Threads::Threads)


target_link_libraries(sys-logger reproc++)
target_link_libraries(sys-logger asio)
target_link_libraries(process1 asio)

