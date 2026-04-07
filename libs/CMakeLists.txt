add_subdirectory(Core)

if(WISKR_BUILD_QTQUICK)
    add_subdirectory(Scenes/QtQuick)
endif()

if(WISKR_BUILD_QTWIDGETS)
    add_subdirectory(Scenes/QtWidgets)
endif()