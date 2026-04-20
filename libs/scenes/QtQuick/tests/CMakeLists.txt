message(STATUS "Building WISKR QtQuick tests")

find_package(Qt${WISKR_QT_MAJOR} REQUIRED COMPONENTS Test)

set(TEST_SOURCES
        unit_tests_main.cpp
        find_qt_item_tests.cpp
        qt_item_tools_tests.cpp
        test_item.cpp
        qt_item_tests.cpp
)

set(TEST_HEADERS
        qml_test_fixture.h
)

add_executable(WISKRQtQuickTests
        ${TEST_SOURCES}
        ${TEST_HEADERS}
)

target_link_libraries(WISKRQtQuickTests
        PRIVATE
        WISKR::QtQuick
        skitr::skitr
        Qt${WISKR_QT_MAJOR}::Test
        GTest::gtest
        GTest::gmock
)

if(WIN32)
    add_custom_command(TARGET WISKRQtQuickTests POST_BUILD
            COMMAND ${Qt6_DIR}/../../../bin/windeployqt.exe
            $<TARGET_FILE:WISKRQtQuickTests>
            COMMENT "Deploying Qt runtime for WISKRQtQuickTests"
    )
endif()

target_include_directories(WISKRQtQuickTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/wiskr
)

add_test(NAME WISKRQtQuickTests COMMAND WISKRQtQuickTests)

include(GoogleTest)
gtest_discover_tests(WISKRQtQuickTests)


set_target_properties(WISKRQtQuickTests PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)