cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE source_files "src/*")
include(FetchContent)

add_library(CommunicationLib ${source_files})
target_include_directories(CommunicationLib PUBLIC "include/")
target_include_directories(CommunicationLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ----------- asio library
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
# ---------------


target_link_libraries(CommunicationLib asio reproc++)