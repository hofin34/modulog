CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 20)

if(NOT TARGET agent_client_lib)
    ADD_SUBDIRECTORY(../../agent-client communication-lib-build)
endif()
ADD_EXECUTABLE(net-connectivity-agent net-connectivity-agent.cpp)
CONFIGURE_FILE(config.json config.json COPYONLY)
TARGET_LINK_LIBRARIES(net-connectivity-agent agent_client_lib)
