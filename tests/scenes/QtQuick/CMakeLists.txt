#
# QtQuick Tests
#
include (FetchContent)

# Force MSVC to use the same CRT as the test binary if needed
set(gtest_force_shared_crt ON CACHE BOOL "")

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)
find_package(Qt${WISKR_QT_MAJOR} COMPONENTS Test REQUIRED)

set(QTQUICK_TEST_SOURCES
        unit_tests_main.cpp
)

add_executable(WISKRQtQuickTests ${QTQUICK_TEST_SOURCES})
target_link_libraries(WISKRQtQuickTests
        PRIVATE
        WISKR::QtQuick
        Qt${WISKR_QT_MAJOR}::Test
        GTest::gtest
        GTest::gmock
)

target_include_directories(WISKRQtQuickTests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

add_test(NAME WISKRQtQuickTests COMMAND WISKRQtQuickTests)