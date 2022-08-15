# What will be compiled:
FILE(READ ../agents-to-compile.json TO_COMPILE_JSON) # do not change to CMAKE_SOURCE_DIR, modulog is built by external CMAKE projects and it doesnt work
STRING(JSON AGENTS_COUNT LENGTH ${TO_COMPILE_JSON} agentsToCompile)
MATH(EXPR AGENTS_COUNT "${AGENTS_COUNT}-1") # Decrementing by one - foreach needs number (count-1)
FILE(REMOVE ${AG_BUILT_LIST}) # list of built agents is deleted - now we can start appending
FOREACH (IDX RANGE ${AGENTS_COUNT})
    # Get the name from the current JSON element.
    STRING(JSON AG_PATH GET ${TO_COMPILE_JSON} agentsToCompile ${IDX} agentPath)
    STRING(JSON AG_ENABLED GET ${TO_COMPILE_JSON} agentsToCompile ${IDX} enabled)
    IF (${AG_ENABLED})
        GET_FILENAME_COMPONENT(AG_NAME ${AG_PATH} NAME)
        ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/${AG_PATH}) # paths in agents-to-compile.json are relative from its destination
        ADD_DEPENDENCIES(modulog ${AG_NAME}) # Agent target has same name as its folder
        FILE(APPEND ${AG_BUILT_LIST} ${AG_PATH} "\n")

        IF(BRINGAUTO_INSTALL)
            INSTALL(TARGETS ${AG_NAME} DESTINATION agents/${AG_NAME})
            INSTALL(FILES ${AG_NAME}/config.json DESTINATION agents/${AG_NAME})
        ENDIF()
    ENDIF ()
ENDFOREACH ()
