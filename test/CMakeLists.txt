CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(Tests)
# GoogleTest requires at least C++11
SET(CMAKE_CXX_STANDARD 17)


MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()


ENABLE_TESTING()
SUBDIRLIST(SUBDIRS smurf-test/agents)
FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()

find_package(Python3 COMPONENTS Interpreter Development)
message("Python_FOUND:${Python3_FOUND}")
message("Python_VERSION:${Python3_VERSION}")
message("Python_VERSION:${Python3_EXECUTABLE}")

add_test(NAME sample_test COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/lib/StateSmurf/SmurfTestingScripts/CompareScenarios.py
        -s ${CMAKE_CURRENT_SOURCE_DIR}/smurf-test/scenarios.json  -e ${CMAKE_BINARY_DIR}/modulog -C
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/smurf-test)# ${EXECUTABLE_OUTPUT_PATH} )

