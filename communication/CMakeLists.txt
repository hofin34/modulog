cmake_minimum_required(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE source_files "src/*")
include(FetchContent)

add_library(CommunicationLib ${source_files})
target_include_directories(CommunicationLib PUBLIC "include/")
target_include_directories(CommunicationLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ----------- nlohmann json library
# TODO downloads too long?
#FetchContent_Declare(json
#        GIT_REPOSITORY https://github.com/nlohmann/json.git
#        GIT_TAG v3.10.4)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
        )

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    ADD_SUBDIRECTORY(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ----------- asio library
FetchContent_Declare(asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
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

# ----------- reproc library
SET(REPROC++ ON)
FetchContent_Declare(
        reproc++
        GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git #https://github.com/hofin34/reproc.git
        GIT_TAG     v14.2.4 #  0bce0977bc3fa9ae10193607f3e27ad04e8aab94
)
FetchContent_MakeAvailable(reproc++)

#-----------

target_link_libraries(CommunicationLib asio reproc++ nlohmann_json::nlohmann_json) # stdc++fs is there because of cross-compiling for Rpi