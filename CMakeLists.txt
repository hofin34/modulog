CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
INCLUDE(FetchContent)
PROJECT(modulog)

OPTION(BRINGAUTO_TESTS "Tests" OFF)
OPTION(BRINGAUTO_INSTALL "Install" OFF)
OPTION(BRINGAUTO_PACKAGE "Create package" OFF)

SET(CMAKE_CXX_STANDARD 17)
SET(AG_BUILT_LIST ${CMAKE_BINARY_DIR}/agents-enabled.conf)

ADD_SUBDIRECTORY(core)

ADD_EXECUTABLE(modulog main.cpp)

# What will be compiled:
FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/agents-to-compile.json TO_COMPILE_JSON)
STRING(JSON AGENTS_COUNT LENGTH ${TO_COMPILE_JSON} agentsToCompile)
MATH(EXPR AGENTS_COUNT "${AGENTS_COUNT}-1") # Decrementing by one - foreach needs number (count-1)
FILE(REMOVE ${AG_BUILT_LIST}) # list of built agents is deleted - now we can start appending
FOREACH (IDX RANGE ${AGENTS_COUNT})
    # Get the name from the current JSON element.
    STRING(JSON AG_PATH GET ${TO_COMPILE_JSON} agentsToCompile ${IDX} agentPath)
    STRING(JSON AG_ENABLED GET ${TO_COMPILE_JSON} agentsToCompile ${IDX} enabled)
    IF (${AG_ENABLED})
        #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output-pack/agents/${AG_NAME})
        GET_FILENAME_COMPONENT(AG_NAME ${AG_PATH} NAME)
        ADD_SUBDIRECTORY(${AG_PATH})
        ADD_DEPENDENCIES(modulog ${AG_NAME}) # Agent target has same name as its folder
        FILE(APPEND ${AG_BUILT_LIST} ${AG_PATH} "\n")

        IF(BRINGAUTO_INSTALL)
            SET(AG_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/agents/${AG_NAME})
            INSTALL(TARGETS ${AG_NAME} DESTINATION ${AG_INSTALL_DIR})
            INSTALL(FILES ${AG_PATH}/config.json DESTINATION ${AG_INSTALL_DIR})
        ENDIF()
    ENDIF ()
ENDFOREACH ()
# ------------ linking

FIND_PACKAGE(libbringauto_logger)
TARGET_LINK_LIBRARIES(modulog CoreLib bringauto_logger::bringauto_logger)
IF(BRINGAUTO_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(test)
ENDIF()

IF(BRINGAUTO_INSTALL)
    INSTALL(TARGETS modulog DESTINATION ".")
    INSTALL(FILES ${AG_BUILT_LIST} DESTINATION ".")
ENDIF()
