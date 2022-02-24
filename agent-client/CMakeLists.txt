CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE source_files "src/*")

ADD_LIBRARY(agent_client_lib ${source_files})
TARGET_INCLUDE_DIRECTORIES(agent_client_lib PUBLIC "include/")
TARGET_INCLUDE_DIRECTORIES(agent_client_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

TARGET_LINK_LIBRARIES(agent_client_lib CommunicationLib)
TARGET_LINK_LIBRARIES(agent_client_lib MetaLib)
