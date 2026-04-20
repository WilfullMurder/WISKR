include(FetchContent)

FetchContent_Declare(
        skitr
        GIT_REPOSITORY https://github.com/WilfullMurder/SKITR.git
        GIT_TAG v0.2.1
)

FetchContent_MakeAvailable(skitr)

add_subdirectory(core)

if(WISKR_BUILD_QTQUICK)
    add_subdirectory(Scenes/QtQuick)
endif()

if(WISKR_BUILD_QTWIDGETS)
    add_subdirectory(Scenes/QtWidgets)
endif()