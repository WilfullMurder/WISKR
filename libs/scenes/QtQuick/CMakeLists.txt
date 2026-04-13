include(GNUInstallDirs)
include(CMakePackageConfigHelpers)


find_package(Qt${WISKR_QT_MAJOR} COMPONENTS
        Core
        Gui
        Quick
        REQUIRED
)
message("Building wiskr QtQuick for Qt ${WISKR_QT_MAJOR}")

file(GLOB HEADERS_PUB_ROOT "include/wiskr/*.h")
set(HEADERS_PUB ${HEADERS_PUB_ROOT})

set(SOURCES
        ${HEADERS_PUB}
        src/QtQmlBot.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCES})

cmake_language(CALL "qt${WISKR_QT_MAJOR}_wrap_cpp" MOC_FILES "include/wiskr/QtQmlBot.h")

add_library(WISKRQtQuick ${SOURCES} ${MOC_FILES})
target_include_directories(WISKRQtQuick
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
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
)

include(GenerateExportHeader)
generate_export_header(WISKRQtQuick
        EXPORT_FILE_NAME wiskr/wiskr_qtquick_export.h
)

install(
        TARGETS WISKRQtQuick
        EXPORT WISKRTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install (DIRECTORY include/wiskr DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wiskr/wiskr_qtquick_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wiskr
)

set(WISKR_QTQUICK_CONFIG_DIR "${CMAKE_INSTALL_DATADIR}/WISKR/qtquick/cmake")

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/WISKRQtQuickConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/WISKRQtQuickConfig.cmake"
        INSTALL_DESTINATION "${WISKR_QTQUICK_CONFIG_DIR}"
)

install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/WISKRQtQuickConfig.cmake"
        DESTINATION "${WISKR_QTQUICK_CONFIG_DIR}"
)

add_library(WISKR::QtQuick ALIAS WISKRQtQuick)
