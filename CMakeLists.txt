cmake_minimum_required(VERSION 3.20)
include(FetchContent)

project(bachelor_hofbauer)

set(CMAKE_CXX_STANDARD 20)

add_executable(sys-logger main.cpp Client.h Client.cpp LogMessage.cpp LogMessage.h ControlMessage.cpp ControlMessage.h TcpConnection.cpp TcpConnection.h TcpServer.cpp TcpServer.h Message.cpp Message.h MessageHandler.cpp MessageHandler.h)
add_executable(process1 process1.cpp Client.h Client.cpp LogMessage.cpp LogMessage.h ControlMessage.cpp ControlMessage.h TcpConnection.cpp TcpConnection.h Message.cpp Message.h MessageHandler.cpp MessageHandler.h)

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
        GIT_TAG        v14.2.3
)
FetchContent_MakeAvailable(reproc++)

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

# ------------ linking
target_link_libraries(sys-logger reproc++ asio nlohmann_json::nlohmann_json)
target_link_libraries(process1 asio)

