CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
INCLUDE(FetchContent)
PROJECT(modulog)

# for linking .so after make install (we have ba_logger):
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

OPTION(BRINGAUTO_TESTS "Tests" OFF)
OPTION(BRINGAUTO_INSTALL "Install" OFF)
OPTION(BRINGAUTO_PACKAGE "Create package" OFF)
OPTION(STATESMURF_TESTS "Testing library StateSmurf" OFF)

IF(BRINGAUTO_TESTS)
    ADD_COMPILE_DEFINITIONS(BRINGAUTO_TESTS=1) # if this macro defined, SmurfState library included in modulog
    IF(BRINGAUTO_PACKAGE OR BRINGAUTO_INSTALL)
        MESSAGE(FATAL_ERROR "You cannot install or create package with BRINGAUTO_TESTS enabled")
    ENDIF()
ENDIF()
IF(BRINGAUTO_PACKAGE)
    IF(NOT BRINGAUTO_INSTALL)
        MESSAGE(WARNING "BRINGAUTO_PACKAGE is ON, so BRINGAUTO_INSTALL is also ON")
        SET(BRINGAUTO_INSTALL ON)
    ENDIF()
ENDIF()

SET(CMAKE_CXX_STANDARD 17)
SET(modulog_VERSION 1.0.0)
SET(AG_BUILT_LIST ${CMAKE_BINARY_DIR}/agents-enabled.conf)


ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(communication)
ADD_SUBDIRECTORY(meta-lib)
ADD_SUBDIRECTORY(agent-client)
ADD_SUBDIRECTORY(agents)

ADD_EXECUTABLE(modulog main.cpp)

# What will be compiled:
FILE(READ ${CMAKE_SOURCE_DIR}/agents-to-compile.json TO_COMPILE_JSON)
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
            INSTALL(TARGETS ${AG_NAME} DESTINATION agents/${AG_NAME})
            INSTALL(FILES ${AG_PATH}/config.json DESTINATION agents/${AG_NAME})
        ENDIF()
    ENDIF ()
ENDFOREACH ()
# ------------ linking

TARGET_LINK_LIBRARIES(modulog CoreLib)
IF(BRINGAUTO_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(lib/StateSmurf)
    ADD_SUBDIRECTORY(test)
ENDIF()

IF(BRINGAUTO_INSTALL)
    INSTALL(TARGETS modulog DESTINATION .)
    INSTALL(FILES ${AG_BUILT_LIST} DESTINATION .)
ENDIF()

IF(BRINGAUTO_PACKAGE)
    SET(CPACK_GENERATOR ZIP)
    SET(CPACK_PACKAGE_CONTACT "Martin Hofbauer <martin.hofbauer@bringauto.com>")
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Martin Hofbauer <martin.hofbauer@bringauto.com>")
    SET(CPACK_PACKAGE_VERSION ${libbringauto_logger_VERSION})
    INCLUDE(CPack)
ENDIF()
