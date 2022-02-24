CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(Tests)
SET(CMAKE_CXX_STANDARD 17)

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children ${curdir}/*)
    SET(dirlist "")
    FOREACH (child ${children})
        IF (IS_DIRECTORY ${child})
            LIST(APPEND dirlist ${child})
        ENDIF ()
    ENDFOREACH ()
    SET(${result} ${dirlist})
ENDMACRO()


ENABLE_TESTING()
SUBDIRLIST(SUBDIRS smurf-test/agents)
FOREACH (subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH ()
CONFIGURE_FILE(smurf-test/scenarios.json ${CMAKE_BINARY_DIR}) # this will replace CMake variables in scenarios.json
FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)
MESSAGE("Python_FOUND:${Python3_FOUND}")
MESSAGE("Python_VERSION:${Python3_VERSION}")
MESSAGE("Python_VERSION:${Python3_EXECUTABLE}")

ADD_TEST(NAME ModulogTests COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/lib/StateSmurf/SmurfTestingScripts/CompareScenarios.py
        -s ${CMAKE_BINARY_DIR}/scenarios.json
        -e ${CMAKE_BINARY_DIR}/modulog
        #-c
        --evaluator ${CMAKE_BINARY_DIR}/lib/StateSmurf/SmurfEvaluator/smurfEvaluator
        --output ${CMAKE_SOURCE_DIR}/test/smurf-test/
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/smurf-test)# ${EXECUTABLE_OUTPUT_PATH} )

