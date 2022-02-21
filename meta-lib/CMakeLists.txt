CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)

FILE(GLOB_RECURSE source_files "src/*")

ADD_LIBRARY(MetaLib ${source_files})
TARGET_INCLUDE_DIRECTORIES(MetaLib PUBLIC "include/")
TARGET_INCLUDE_DIRECTORIES(MetaLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

IF(BRINGAUTO_TESTS)
    ADD_SUBDIRECTORY(../lib/StateSmurf state-smurf-build)
    TARGET_LINK_LIBRARIES(MetaLib PUBLIC DiagramSmurfLib TransitionSmurfLib)
ENDIF()

execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/lib/ba-logger/libbringauto_logger-dev_v1.1.0_x86-64-ubuntu-2004.zip
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

FIND_PACKAGE(libbringauto_logger PATHS ${CMAKE_BINARY_DIR})
TARGET_LINK_LIBRARIES(MetaLib PUBLIC bringauto_logger::bringauto_logger)
IF(BRINGAUTO_INSTALL)
    INSTALL(IMPORTED_RUNTIME_ARTIFACTS bringauto_logger::bringauto_logger_spdlog DESTINATION lib)
ENDIF()
