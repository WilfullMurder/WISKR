if(WISKR_BUILD_TESTS)
    include (FetchContent)

    # Force MSVC to use the same CRT as the test binary if needed
    set(gtest_force_shared_crt ON CACHE BOOL "")

    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.17.0
    )
    FetchContent_MakeAvailable(googletest)

    add_library(gtest::gtest ALIAS gtest)
    add_library(gtest::gmock ALIAS gmock)
endif()


add_subdirectory(core)


if(WISKR_BUILD_QTQUICK)
    add_subdirectory(Scenes/QtQuick)
endif()


if(WISKR_BUILD_QTWIDGETS)
    add_subdirectory(Scenes/QtWidgets)
endif()