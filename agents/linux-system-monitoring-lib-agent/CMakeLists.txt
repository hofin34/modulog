CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)
SET(AG_NAME linux-system-monitoring-lib-agent)

FetchContent_Declare(linuxmonitoring
        GIT_REPOSITORY git@github.com:hofin34/Linux-System-Monitoring-Library.git
        GIT_TAG origin/master
        )
FetchContent_MakeAvailable(linuxmonitoring)

ADD_EXECUTABLE(${AG_NAME} ${AG_NAME}.cpp)
CONFIGURE_FILE(config.json config.json COPYONLY)
TARGET_LINK_LIBRARIES(${AG_NAME} agent_client_lib)
TARGET_LINK_LIBRARIES(${AG_NAME} linuxmonitoring)
IF(BRINGAUTO_INSTALL)
    INSTALL(IMPORTED_RUNTIME_ARTIFACTS linuxmonitoring DESTINATION lib)
ENDIF()
