CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/logs") # default folder for logs

FILE(GLOB_RECURSE source_files "src/*")
ADD_LIBRARY(CoreLib ${source_files})
TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC "include/")


TARGET_LINK_LIBRARIES(CoreLib CommunicationLib )
