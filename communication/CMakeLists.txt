CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)

FILE(GLOB_RECURSE source_files "src/*")
INCLUDE(FetchContent)

ADD_LIBRARY(CommunicationLib ${source_files})
TARGET_INCLUDE_DIRECTORIES(CommunicationLib PUBLIC "include/")
TARGET_INCLUDE_DIRECTORIES(CommunicationLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ----------- nlohmann json library
# TODO downloads too long?
#FetchContent_Declare(json
#        GIT_REPOSITORY https://github.com/nlohmann/json.git
#        GIT_TAG v3.10.4)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
        )

FetchContent_GetProperties(json)
IF(NOT json_POPULATED)
    FetchContent_Populate(json)
    ADD_SUBDIRECTORY(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()

# ----------- asio library
FetchContent_Declare(asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG  asio-1-20-0
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        )
FetchContent_GetProperties(asio)
IF(NOT asio_POPULATED)
    FetchContent_Populate(asio)
ENDIF()
ADD_LIBRARY(asio INTERFACE)
TARGET_INCLUDE_DIRECTORIES(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
FIND_PACKAGE(Threads)
TARGET_LINK_LIBRARIES(asio INTERFACE Threads::Threads)

# ----------- reproc library
SET(REPROC++ ON)
FetchContent_Declare(
        reproc++
        GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git #https://github.com/hofin34/reproc.git
        GIT_TAG     v14.2.4 #  0bce0977bc3fa9ae10193607f3e27ad04e8aab94
)
FetchContent_MakeAvailable(reproc++)

#-----------

TARGET_LINK_LIBRARIES(CommunicationLib asio reproc++ nlohmann_json::nlohmann_json MetaLib)