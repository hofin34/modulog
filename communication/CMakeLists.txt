CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)

FILE(GLOB_RECURSE source_files "src/*")

ADD_LIBRARY(CommunicationLib ${source_files})
TARGET_INCLUDE_DIRECTORIES(CommunicationLib PUBLIC "include/")
TARGET_INCLUDE_DIRECTORIES(CommunicationLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


TARGET_LINK_LIBRARIES(CommunicationLib asio reproc++ nlohmann_json::nlohmann_json MetaLib)