include (FetchContent)

# Force MSVC to use the same CRT as the test binary if needed
set(gtest_force_shared_crt ON CACHE BOOL "")

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

# Recursively gather all .cpp test sources in this tests tree
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
# Keep IDE tree structure matching filesystem
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX tests FILES ${TEST_SOURCES})

add_library(wiskr_test_support
        mocks/mock_scene.cpp
        mocks/mock_item.cpp
        mocks/mock_events.cpp
)

target_include_directories(wiskr_test_support
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  # so tests can include "mocks/..."
)

target_link_libraries(wiskr_test_support
        PUBLIC
        WISKR::Core
)

add_executable(wiskr_tests ${TEST_SOURCES})

target_link_libraries(wiskr_tests
        PRIVATE
        WISKR::Core
        wiskr_test_support
        GTest::gtest_main
)

# Optional: match warning flags used by the library
if (MSVC)
    target_compile_options(wiskr_tests PRIVATE /W4)
else()
    target_compile_options(wiskr_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Register with CTest
add_test(NAME wiskr_tests COMMAND wiskr_tests)