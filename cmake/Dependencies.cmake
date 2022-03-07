INCLUDE(FetchContent)
FIND_PACKAGE(CMLIB REQUIRED)

# ----------- BringAuto logger
#CMLIB_DEPENDENCY(
#        URI "https://github.com/bringauto/balogger-package.git"
#        URI_TYPE GIT
#        GIT_TAG v1.1.0
#        TYPE MODULE
#)
#FIND_PACKAGE(balogger_package REQUIRED)
#FIND_PACKAGE(libbringauto_logger)

SET(BALOGGER_ZIP none)
IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    message("Architecture: aarch ${CMAKE_SYSTEM_PROCESSOR}")
    SET(BALOGGER_ZIP ${CMAKE_SOURCE_DIR}/lib/ba-logger/libbringauto_logger-dev_v1.1.0_aarch64-ubuntu-1804.zip)
    LIST(APPEND CMAKE_FIND_ROOT_PATH ${CMAKE_BINARY_DIR})
ELSE()
    message("Architecture: else ${CMAKE_SYSTEM_PROCESSOR}")
    SET(BALOGGER_ZIP ${CMAKE_SOURCE_DIR}/lib/ba-logger/libbringauto_logger-dev_v1.1.0_x86-64-ubuntu-2004.zip)
ENDIF()

EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -E tar xzf  ${BALOGGER_ZIP}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

message("adsf: ${CMAKE_FIND_ROOT_PATH}")
#message("BD: ${CMAKE_BINARY_DIR}")
FIND_PACKAGE(libbringauto_logger PATHS ${CMAKE_BINARY_DIR})
message("xxx: ${libbringauto_logger_DIR}")

IF(BRINGAUTO_INSTALL)
    INSTALL(IMPORTED_RUNTIME_ARTIFACTS bringauto_logger::bringauto_logger_spdlog DESTINATION lib)
ENDIF()

# ----------- nlohmann json library
FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
        )

FetchContent_GetProperties(json)
IF (NOT json_POPULATED)
    FetchContent_Populate(json)
    ADD_SUBDIRECTORY(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF ()

# ----------- asio library
FetchContent_Declare(asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-20-0
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        )
FetchContent_GetProperties(asio)
IF (NOT asio_POPULATED)
    FetchContent_Populate(asio)
ENDIF ()
ADD_LIBRARY(asio INTERFACE)
TARGET_INCLUDE_DIRECTORIES(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
FIND_PACKAGE(Threads)
TARGET_LINK_LIBRARIES(asio INTERFACE Threads::Threads)

# ----------- reproc library
SET(REPROC++ ON)
FetchContent_Declare(
        reproc++
        GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git #https://github.com/hofin34/reproc.git
        GIT_TAG v14.2.4 #  0bce0977bc3fa9ae10193607f3e27ad04e8aab94
)
FetchContent_MakeAvailable(reproc++)



# ----------- cxxopts
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG        v3.0.0
        GIT_SHALLOW    TRUE
)
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_WARNINGS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cxxopts)
