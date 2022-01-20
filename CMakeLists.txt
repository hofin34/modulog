CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
INCLUDE(FetchContent)
PROJECT(bachelor_hofbauer)

SET(CMAKE_CXX_STANDARD 20)

if (NOT TARGET CommunicationLib)
    ADD_SUBDIRECTORY(communication)
endif()

FILE(GLOB_RECURSE source_files "core/src/*")
ADD_EXECUTABLE(modulog main.cpp ${source_files})
TARGET_INCLUDE_DIRECTORIES(modulog PUBLIC "core/include/")
TARGET_LINK_LIBRARIES(modulog CommunicationLib)

ADD_SUBDIRECTORY(agents/net-connectivity-agent)
ADD_SUBDIRECTORY(agents/ram-agent)
ADD_SUBDIRECTORY(agents/uptime-agent)
ADD_SUBDIRECTORY(agents/space-agent)
ADD_SUBDIRECTORY(agents/test-random-agent)
ADD_SUBDIRECTORY(agents/temperature-agent)
ADD_SUBDIRECTORY(agents/smart-agent)

ADD_DEPENDENCIES(modulog net-connectivity-agent ram-agent uptime-agent space-agent test-random-agent temperature-agent smart-agent)

# ----------- nlohmann json library
# TODO downloads too long?
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.4)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    ADD_SUBDIRECTORY(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()



# ------------ linking
TARGET_LINK_LIBRARIES(modulog reproc++ asio nlohmann_json::nlohmann_json)


