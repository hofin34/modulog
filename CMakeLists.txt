cmake_minimum_required(VERSION 3.20)
include(FetchContent)
project(bachelor_hofbauer)

set(CMAKE_CXX_STANDARD 20)

if (NOT TARGET CommunicationLib)
    add_subdirectory(communication)
endif()

file(GLOB_RECURSE source_files "core/src/*")
add_executable(sys-logger main.cpp ${source_files})
target_include_directories(sys-logger PUBLIC "core/include/")
target_link_libraries(sys-logger CommunicationLib)

add_subdirectory(agents/ram-logger)
add_subdirectory(agents/uptime-logger)
add_subdirectory(agents/space-logger)
add_subdirectory(agents/test-random-logger)
add_subdirectory(agents/temperature-logger)

add_dependencies(sys-logger ram-logger uptime-logger space-logger test-random-logger temperature-logger)
# ----------- nlohmann json library
# TODO downloads too long?
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.4)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


# ----------- reproc library
set(REPROC++ ON)
FetchContent_Declare(
        reproc++
        GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
        GIT_TAG        v14.2.4
)
FetchContent_MakeAvailable(reproc++)


# ------------ linking
target_link_libraries(sys-logger reproc++ asio nlohmann_json::nlohmann_json)


