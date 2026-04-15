#
# QtQuick Tests
#

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