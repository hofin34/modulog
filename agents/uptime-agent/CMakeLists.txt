CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)
SET(AG_NAME uptime-agent)
ADD_EXECUTABLE(${AG_NAME} ${AG_NAME}.cpp)
CONFIGURE_FILE(config.json config.json COPYONLY)
TARGET_LINK_LIBRARIES(${AG_NAME} agent_client_lib)
