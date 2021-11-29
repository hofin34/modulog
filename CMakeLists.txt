cmake_minimum_required(VERSION 3.20)
include(FetchContent)
project(bachelor_hofbauer)

set(CMAKE_CXX_STANDARD 20)

if (NOT TARGET CommunicationLib)
    add_subdirectory(communication)
endif()
add_subdirectory(agents/memory-logger)
add_subdirectory(agents/uptime-logger)


add_executable(sys-logger main.cpp communication/src/LogMessage.cpp communication/include/LogMessage.h communication/src/ControlMessage.cpp communication/include/ControlMessage.h communication/src/TcpConnection.cpp communication/include/TcpConnection.h communication/src/TcpServer.cpp communication/include/TcpServer.h communication/src/Message.cpp communication/include/Message.h communication/src/MessageSerializer.cpp communication/include/MessageSerializer.h communication/src/MessageDeserializer.cpp communication/include/MessageDeserializer.h core/src/Agent.cpp core/include/Agent.h core/src/AgentHandler.cpp core/include/AgentHandler.h core/src/Core.cpp core/include/Core.h core/src/LogSaver.cpp core/include/LogSaver.h communication/src/AgentClient.cpp communication/include/AgentClient.h)

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


# ------------ linking
target_link_libraries(sys-logger reproc++ asio nlohmann_json::nlohmann_json)


