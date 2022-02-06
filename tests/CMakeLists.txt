CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(Tests)

# GoogleTest requires at least C++11
SET(CMAKE_CXX_STANDARD 20)

INCLUDE(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

ENABLE_TESTING()

ADD_EXECUTABLE(hello_test hello_test.cpp)
TARGET_LINK_LIBRARIES(hello_test gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)
