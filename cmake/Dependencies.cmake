INCLUDE(FetchContent)
IF (NOT BRINGAUTO_SYSTEM_DEP AND NOT BRINGAUTO_BUILD_DEP)
    FIND_PACKAGE(CMLIB REQUIRED)
ENDIF ()

# where to search for libs (there was problem when crosscompiling):
LIST(APPEND CMAKE_FIND_ROOT_PATH ${CMAKE_BINARY_DIR})
LIST(APPEND CMAKE_FIND_ROOT_PATH ${CMLIB_REQUIRED_ENV_TMP_PATH})


FUNCTION(ADD_DEP_BRINGAUTO_LOGGER)
    MESSAGE("Adding ba-logger...")
    # if using FetContent, then we are linking against bringauto_logger, otherwise bringauto_logger::bringauto_logger
    SET(BRINGAUTO_LOGGER_TO_LINK bringauto_logger::bringauto_logger PARENT_SCOPE)
    SET(BRINGAUTO_LOGGER_TO_INSTALL bringauto_logger::bringauto_logger_spdlog)
    IF (BRINGAUTO_BUILD_DEP)
        SET(BRINGAUTO_LOGGER_TO_LINK bringauto_logger PARENT_SCOPE)
        SET(BRINGAUTO_LOGGER_TO_INSTALL bringauto_logger_spdlog)
        SET(BRINGAUTO_SYSTEM_DEP_SAVED ${BRINGAUTO_SYSTEM_DEP})
        SET(BRINGAUTO_SYSTEM_DEP ON)
        SET(BRINGAUTO_INSTALL_SAVED ${BRINGAUTO_INSTALL})
        SET(BRINGAUTO_INSTALL ON) # has to be turned on, ba logger without this will not generate everything necessary
        FetchContent_Declare(
                spdlog
                GIT_REPOSITORY https://github.com/gabime/spdlog.git
                GIT_TAG v1.8.5
        )
        FetchContent_MakeAvailable(spdlog)
        SET(LIB_TYPE SPDLOG)
        FetchContent_Declare(
                balogger
                GIT_REPOSITORY https://github.com/bringauto/ba-logger.git
                GIT_TAG v1.1.1
        )
        FetchContent_MakeAvailable(balogger)
        SET(BRINGAUTO_SYSTEM_DEP ${BRINGAUTO_SYSTEM_DEP_SAVED})
        SET(BRINGAUTO_INSTALL ${BRINGAUTO_INSTALL_SAVED})
    ELSEIF(BRINGAUTO_SYSTEM_DEP)
        FIND_PACKAGE(libbringauto_logger)
    ELSE()
        SET(BRINGAUTO_LOGGER_TO_LINK bringauto_logger PARENT_SCOPE)
        SET(BRINGAUTO_LOGGER_TO_INSTALL bringauto_logger_spdlog)
        CMLIB_DEPENDENCY(
                URI "https://github.com/bringauto/balogger-package.git"
                URI_TYPE GIT
                GIT_TAG v1.1.0
                TYPE MODULE
        )
        FIND_PACKAGE(balogger_package REQUIRED)
    ENDIF()

    IF (BRINGAUTO_INSTALL)
        INSTALL(IMPORTED_RUNTIME_ARTIFACTS ${BRINGAUTO_LOGGER_TO_INSTALL} DESTINATION lib)
    ENDIF ()
ENDFUNCTION()

FUNCTION(ADD_DEP_NLOHMANN_JSON)
    MESSAGE("Adding nlohmann json...")
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    IF (BRINGAUTO_BUILD_DEP)
        FetchContent_Declare(json
                URL https://github.com/nlohmann/json/archive/refs/tags/v3.9.1.tar.gz
                )

        FetchContent_GetProperties(json)
        IF (NOT json_POPULATED)
            FetchContent_Populate(json)
            ADD_SUBDIRECTORY(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
        ENDIF ()
    ELSEIF (BRINGAUTO_SYSTEM_DEP)
        FIND_PACKAGE(nlohmann_json)
    ELSE ()
        CMLIB_DEPENDENCY(
                URI "https://github.com/bringauto/nlohmann_json-package.git"
                URI_TYPE GIT
                GIT_TAG v3.9.1
                TYPE MODULE
        )
        FIND_PACKAGE(nlohmann_json_package REQUIRED)
        FIND_PACKAGE(nlohmann_json)
    ENDIF ()
ENDFUNCTION()


FUNCTION(ADD_DEP_ASIO)
    MESSAGE("Adding asio... - adding through FetchContent")
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
ENDFUNCTION()

FUNCTION(ADD_DEP_REPROC)
    MESSAGE("Adding reproc...- through FetchContent")
    SET(REPROC++ ON)
    SET(REPROC_TEST OFF)
    SET(REPROC_EXAMPLES OFF)
    FetchContent_Declare(
            reproc++
            GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
            GIT_TAG v14.2.4
    )
    FetchContent_MakeAvailable(reproc++)
ENDFUNCTION()


FUNCTION(ADD_DEP_CXXOPTS)
    MESSAGE("Adding cxxopts...")
    IF(BRINGAUTO_BUILD_DEP)
        FetchContent_Declare(
                cxxopts
                GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
                GIT_TAG v3.0.0
                GIT_SHALLOW TRUE
        )
        SET(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        SET(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        SET(CXXOPTS_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
        SET(CXXOPTS_ENABLE_WARNINGS OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(cxxopts)
    ELSEIF(BRINGAUTO_SYSTEM_DEP)
        FIND_PACKAGE(cxxopts)
    ELSE ()
        CMLIB_DEPENDENCY(
                URI "https://github.com/bringauto/cxxopts-package.git"
                URI_TYPE GIT
                GIT_TAG v3.0.0
                TYPE MODULE
        )
        FIND_PACKAGE(cxxopts_package 3.0.0 REQUIRED)
        FIND_PACKAGE(cxxopts)
    ENDIF ()
ENDFUNCTION()

FUNCTION(ADD_DEP_STATE_SMURF)
    MESSAGE("Adding StateSmurf...")
    FetchContent_Declare(
            statesmurf
            GIT_REPOSITORY https://github.com/Melky-Phoe/StateSmurf.git
            GIT_TAG v0.1.0
            )
ENDFUNCTION()


ADD_DEP_BRINGAUTO_LOGGER()
ADD_DEP_NLOHMANN_JSON()
ADD_DEP_ASIO()
ADD_DEP_REPROC()
ADD_DEP_CXXOPTS()
IF (BRINGAUTO_TESTS)
    ADD_DEP_STATE_SMURF()
ENDIF ()