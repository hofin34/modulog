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

#ADD_SUBDIRECTORY(agents/net-connectivity-agent)
ADD_SUBDIRECTORY(agents/ram-agent)
ADD_SUBDIRECTORY(agents/uptime-agent)
#ADD_SUBDIRECTORY(agents/space-agent)
ADD_SUBDIRECTORY(agents/test-random-agent)
#ADD_SUBDIRECTORY(agents/temperature-agent)
#ADD_SUBDIRECTORY(agents/smart-agent)

ADD_DEPENDENCIES(modulog ram-agent uptime-agent test-random-agent)#net-connectivity-agent space-agent temperature-agent smart-agent)





# ------------ linking
TARGET_LINK_LIBRARIES(modulog reproc++ asio nlohmann_json::nlohmann_json)


