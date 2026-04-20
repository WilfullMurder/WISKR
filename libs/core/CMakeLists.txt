include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

find_package(Threads REQUIRED)

# =========================
# Core Library
# =========================

file(GLOB HEADERS_PUB_ROOT "include/WISKR/*.h")
file(GLOB HEADERS_PUB_SUB "include/WISKR/**/*.h")

set(SOURCES
        ${HEADERS_PUB_ROOT}
        ${HEADERS_PUB_SUB}
        src/test_server.cpp
        src/skitr_server.cpp

        src/commands/command.cpp
        src/commands/click_on_item.cpp
        src/commands/custom_command.cpp
        src/commands/drag_begin.cpp
        src/commands/drag_end.cpp
        src/commands/drop_from_ext.cpp
        src/commands/enter_key.cpp
        src/commands/exists_and_visible.cpp
        src/commands/get_bounding_box.cpp
        src/commands/get_property.cpp
        src/commands/get_test_status.cpp
        src/commands/input_text.cpp
        src/commands/invoke_method.cpp
        src/commands/quit.cpp
        src/commands/screenshot.cpp
        src/commands/screenshot_base64.cpp
        src/commands/set_property.cpp
        src/commands/wait.cpp
        src/commands/wait_for_item.cpp

        src/data/geometry.cpp
        src/data/item_path.cpp
        src/data/item_path_component.cpp
        src/data/item_postion.cpp
        src/data/pasteboard_content.cpp

        src/dispatch/environment.cpp
        src/dispatch/state.cpp
        src/dispatch/dispatcher.cpp

        src/utils/path_parser.cpp
)

add_library(WISKRCore ${SOURCES})

include(GenerateExportHeader)
generate_export_header(WISKRCore
        EXPORT_FILE_NAME wiskr/wiskr_core_export.h
)

target_include_directories(WISKRCore
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        src
)

target_compile_definitions(WISKRCore PUBLIC ASIO_STANDALONE)

target_link_libraries(WISKRCore
        PUBLIC
        Threads::Threads
        PRIVATE
        skitr::skitr
)

# =========================
# Platform deps
# =========================
if(WIN32)
    target_link_libraries(WISKRCore PUBLIC ws2_32 mswsock)
endif()

# =========================
# Install
# =========================

install(TARGETS WISKRCore
        EXPORT WISKRTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/WISKR
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/WISKR/wiskr_core_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/WISKR
)

set(WISKR_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_DATADIR}/WISKR/cmake")

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/WISKRCoreConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/WISKRCoreConfig.cmake"
        INSTALL_DESTINATION "${WISKR_CMAKE_CONFIG_DIR}"
)

install(EXPORT WISKRTargets
        FILE WISKRTargets.cmake
        DESTINATION "${WISKR_CMAKE_CONFIG_DIR}"
        NAMESPACE WISKR::
)

install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/WISKRCoreConfig.cmake"
        DESTINATION "${WISKR_CMAKE_CONFIG_DIR}"
)

add_library(WISKR::Core ALIAS WISKRCore)

if(WISKR_BUILD_TESTS)
    add_subdirectory(tests)
endif()