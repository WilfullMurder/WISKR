cmake_minimum_required(VERSION 3.28)
project(wiskr LANGUAGES CXX)



# -----------------------------------------------------------------------------
# Global configuration (ORCHESTRATION ONLY)
# -----------------------------------------------------------------------------

option(WISKR_BUILD_EXAMPLES "Build examples" OFF)
option(WISKR_BUILD_TESTS "Build tests" ON)
option(WISKR_BUILD_QTQUICK "Build the QtQuick scene lib" OFF)
option(WISKR_BUILD_QTWIDGETS "Build the QtWidgets scene lib" OFF)

set(WISKR_QT_MAJOR "6" CACHE STRING "Qt major version")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Qt discovery (only orchestration)
# -----------------------------------------------------------------------------

if(DEFINED ENV{QT6_ROOT} AND NOT DEFINED Qt6_DIR)
    list(APPEND CMAKE_PREFIX_PATH "$ENV{QT6_ROOT}")
endif()


# -----------------------------------------------------------------------------
# Subprojects
# -----------------------------------------------------------------------------

add_subdirectory(libs)

if(WISKR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ONLY orchestration: enable testing, nothing else
if(WISKR_BUILD_TESTS)
    enable_testing()
endif()


# -----------------------------------------------------------------------------
# Install config
# -----------------------------------------------------------------------------

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