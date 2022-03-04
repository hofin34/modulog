CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(CMAKE_CXX_STANDARD 17)
SET(AG_NAME linux-system-monitoring-lib-agent)

FetchContent_Declare(linuxmonitoring
        GIT_REPOSITORY git@github.com:hofin34/Linux-System-Monitoring-Library.git
        GIT_TAG 18839dc964ed8a34d3e06eb5be7eac395b9a15ed
        )
FetchContent_GetProperties(linuxmonitoring)
if(NOT linuxmonitoring_POPULATED)
    FetchContent_Populate(linuxmonitoring)
    add_subdirectory(${linuxmonitoring_SOURCE_DIR} ${linuxmonitoring_BINARY_DIR})
    TARGET_INCLUDE_DIRECTORIES(linuxmonitoring PUBLIC "${linuxmonitoring_SOURCE_DIR}/lib/")
    TARGET_INCLUDE_DIRECTORIES(linuxmonitoring PUBLIC "${linuxmonitoring_SOURCE_DIR}/lib/util/")
endif()

ADD_EXECUTABLE(${AG_NAME} ${AG_NAME}.cpp)
CONFIGURE_FILE(config.json config.json COPYONLY)
TARGET_LINK_LIBRARIES(${AG_NAME} agent_client_lib)


#FetchContent_GetProperties(linuxmonitoring)
#if (NOT linuxmonitoring_POPULATED)
#    FetchContent_Populate(linuxmonitoring)
#    INCLUDE(${linuxmonitoring_SOURCE_DIR}/CMakeLists.txt ${linuxmonitoring_BINARY_DIR})
#endif()
#INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/lib/Linux-System-Monitoring-Library/CMakeLists.txt)
TARGET_LINK_LIBRARIES(${AG_NAME} linuxmonitoring)
IF(BRINGAUTO_INSTALL)
    INSTALL(IMPORTED_RUNTIME_ARTIFACTS linuxmonitoring DESTINATION lib)
ENDIF()
