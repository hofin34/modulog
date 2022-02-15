CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(Tests)
# GoogleTest requires at least C++11
SET(CMAKE_CXX_STANDARD 17)

ENABLE_TESTING()
INCLUDE(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

ADD_EXECUTABLE(hello_test hello_test.cpp)
TARGET_LINK_LIBRARIES(hello_test gtest_main CoreLib)
ADD_SUBDIRECTORY(agents/exiting-agent)
ADD_SUBDIRECTORY(agents/simple-log-agent)
INCLUDE(GoogleTest)
gtest_discover_tests(hello_test)

ADD_TEST(NAME hello_test
        COMMAND hello_test)
CONFIGURE_FILE(agents-enabled.conf agents-enabled.conf)

