CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)

IF(NOT TARGET CommunicationLib)
    ADD_SUBDIRECTORY(../communication communication-build)
ENDIF()

FILE(GLOB_RECURSE source_files "src/*")
ADD_LIBRARY(CoreLib ${source_files})
TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC "include/")


TARGET_LINK_LIBRARIES(CoreLib CommunicationLib )
