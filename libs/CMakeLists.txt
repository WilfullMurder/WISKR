add_subdirectory(Core)


add_subdirectory(Scenes/QtQuick)


if(WISKR_BUILD_QTWIDGETS)
    add_subdirectory(Scenes/QtWidgets)
endif()