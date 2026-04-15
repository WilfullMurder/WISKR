cmake_minimum_required(VERSION 3.28)
project(wiskr LANGUAGES CXX)

if(DEFINED ENV{QT6_ROOT} AND NOT DEFINED Qt6_DIR)
    list(APPEND CMAKE_PREFIX_PATH "$ENV{QT6_ROOT}")
endif()

if(DEFINED ENV{Qt6_DIR} AND NOT DEFINED Qt6_DIR)
    set(Qt6_DIR "$ENV{Qt6_DIR}" CACHE PATH "Qt6 cmake dir from environment")
endif()

option(WISKR_BUILD_EXAMPLES "Build examples" OFF)
option(WISKR_BUILD_TESTS "Build tests" ON)
option(WISKR_BUILD_QTQUICK "Build the QtQuick scene lib" OFF)
option(WISKR_BUILD_QTWIDGETS "Build the QtWidgets scene lib" OFF)
set(WISKR_QT_MAJOR "6" CACHE STRING "Major Qt version to build WISKR against")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")
# Force policy minimum to 3.5 to satisfy older CMakeLists of deps
cmake_policy(VERSION 3.28)               # Already sets policies to 3.28 defaults
cmake_policy(SET CMP0077 NEW)            # optional: make FetchContent work nicely
# This ensures FetchContent deps with old cmake_minimum_required don't complain
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)

# Hide symbols unless explicitly flagged with WISKR_EXPORT
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_subdirectory(libs)

if(WISKR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(WISKR_BUILD_TESTS)
    enable_testing()
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/WISKRConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/WISKRConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/WISKR/cmake"
)

install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/WISKRConfig.cmake"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/WISKR/cmake"
)