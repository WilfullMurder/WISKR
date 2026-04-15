include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(Threads REQUIRED)
include(FetchContent)

# Asio
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-28-0
)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(asio nlohmann_json)

add_library(asio::asio INTERFACE IMPORTED)
set_property(TARGET asio::asio PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES "${asio_SOURCE_DIR}/asio/include"
)
set_property(TARGET asio::asio PROPERTY
        INTERFACE_COMPILE_DEFINITIONS ASIO_STANDALONE
)

# SKITR
FetchContent_Declare(
        skitr
        GIT_REPOSITORY https://github.com/WilfullMurder/SKITR.git
        GIT_TAG v0.2.0
)
FetchContent_MakeAvailable(skitr)

file(GLOB HEADERS_PUB_ROOT "include/WISKR/*.h")
file(GLOB HEADERS_PUB_SUB "include/WISKR/**/*.h")
set(HEADERS_PUB ${HEADERS_PUB_ROOT} ${HEADERS_PUB_SUB})

set(SOURCES
        ${HEADERS_PUB}
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

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCES})

add_library(WISKRCore ${SOURCES})

include(GenerateExportHeader)
generate_export_header(WISKRCore
        EXPORT_FILE_NAME wiskr/wiskr_core_export.h
)

target_include_directories(WISKRCore
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${asio_SOURCE_DIR}/asio/include>
        $<BUILD_INTERFACE:${skitr_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${skitr_BINARY_DIR}> # <-- Needed for skitr_export.h
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>   # <-- Needed for wiskr_core_export.h
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        src
)

# If nlohmann_json was fetched, add its include dir to the build interface without depending on an exportable target
if(DEFINED nlohmann_json_SOURCE_DIR)
    target_include_directories(WISKRCore PUBLIC $<BUILD_INTERFACE:${nlohmann_json_SOURCE_DIR}/include>)
endif()

target_compile_definitions(WISKRCore PUBLIC ASIO_STANDALONE)

if(MSVC)
    target_compile_options(WISKRCore PRIVATE /W4)
else()
    target_compile_options(WISKRCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    target_link_libraries(WISKRCore PUBLIC ws2_32 mswsock)
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")


target_link_libraries(WISKRCore
        PUBLIC
        Threads::Threads
        asio::asio
)

if(TARGET skitr::skitr)
    target_link_libraries(WISKRCore PRIVATE skitr::skitr)
elseif(TARGET skitr)
    target_link_libraries(WISKRCore PRIVATE skitr)
endif()

install(
        TARGETS
        WISKRCore
        EXPORT WISKRTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/WISKR DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/WISKR/WISKRCore_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/WISKR)

set(WISKR_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_DATADIR}/WISKR/cmake")

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/WISKRCoreConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/WISKRCoreConfig.cmake"
        INSTALL_DESTINATION "${WISKR_CMAKE_CONFIG_DIR}"
)

install(
        EXPORT WISKRTargets
        FILE WISKRTargets.cmake
        DESTINATION "${CMAKE_INSTALL_DATADIR}/WISKR/cmake"
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