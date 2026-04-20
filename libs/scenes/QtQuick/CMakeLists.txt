if(WISKR_BUILD_TESTS)
    add_subdirectory(tests)
endif()

find_package(Qt${WISKR_QT_MAJOR} REQUIRED COMPONENTS Core Gui Quick Qml)

file(GLOB HEADERS_PUB_ROOT "include/wiskr/*.h")
file(GLOB HEADERS_PUB_SUB "include/wiskr/**/*.h")

set(SOURCES
        ${HEADERS_PUB_ROOT}
        ${HEADERS_PUB_SUB}

        src/find_qt_item.cpp
        src/qt_events.cpp
        src/qt_item.cpp
        src/qt_item_tools.cpp
        src/qt_scene.cpp
        src/QtQmlBot.cpp
        src/utils/qt_event_recorder.cpp
        src/utils/debug_dump.cpp
)

add_library(WISKRQtQuick ${SOURCES})

include(GenerateExportHeader)

generate_export_header(WISKRQtQuick
        EXPORT_FILE_NAME wiskr/wiskr_qtquick_export.h
)

target_include_directories(WISKRQtQuick
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        src
)

target_link_libraries(WISKRQtQuick
        PUBLIC
        WISKR::Core
        Qt${WISKR_QT_MAJOR}::Core
        Qt${WISKR_QT_MAJOR}::Gui
        Qt${WISKR_QT_MAJOR}::Quick
        Qt${WISKR_QT_MAJOR}::Qml
        skitr::skitr
)

set_target_properties(WISKRQtQuick PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)

add_library(WISKR::QtQuick ALIAS WISKRQtQuick)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wiskr/wiskr_qtquick_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wiskr
)