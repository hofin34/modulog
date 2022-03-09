CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)
SET(AG_NAME smart-agent)


FetchContent_Declare(
        smartctl
        GIT_REPOSITORY git@github.com:smartmontools/smartmontools.git
        GIT_TAG RELEASE_7_2 #  0bce0977bc3fa9ae10193607f3e27ad04e8aab94
)
FetchContent_GetProperties(smartctl)
IF (NOT smartctl_POPULATED)
    FetchContent_Populate(smartctl)
ENDIF ()

IF(NOT EXISTS ${smartctl_SOURCE_DIR}/smartmontools/Makefile)
    EXECUTE_PROCESS(COMMAND ./autogen.sh
            WORKING_DIRECTORY ${smartctl_SOURCE_DIR}/smartmontools)
    EXECUTE_PROCESS(COMMAND ./configure
            WORKING_DIRECTORY ${smartctl_SOURCE_DIR}/smartmontools)
    EXECUTE_PROCESS(COMMAND make
            WORKING_DIRECTORY ${smartctl_SOURCE_DIR}/smartmontools)
ENDIF()

CONFIGURE_FILE(${smartctl_SOURCE_DIR}/smartmontools/smartctl smartctl COPYONLY)

ADD_EXECUTABLE(${AG_NAME} ${AG_NAME}.cpp)
CONFIGURE_FILE(config.json config.json COPYONLY)
TARGET_LINK_LIBRARIES(${AG_NAME} agent_client_lib)
