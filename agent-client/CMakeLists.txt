CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE source_files "src/*")

ADD_LIBRARY(agent_client_lib ${source_files})
TARGET_INCLUDE_DIRECTORIES(agent_client_lib PUBLIC "include/")
TARGET_INCLUDE_DIRECTORIES(agent_client_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT TARGET CommunicationLib)
    ADD_SUBDIRECTORY(../communication communication-lib-build)
endif()
TARGET_LINK_LIBRARIES(agent_client_lib CommunicationLib)
