message(STATUS ">>> ENTERING core/tests CMakeLists.txt")



set(CORE_TEST_SOURCES
        unit_tests_main.cpp

        commands/click_on_item_tests.cpp
        commands/custom_command_tests.cpp
        commands/drag_begin_tests.cpp
        commands/drag_end_tests.cpp
        commands/enter_key_tests.cpp
        commands/exists_and_visible_tests.cpp
        commands/get_bounding_box_tests.cpp
        commands/get_property_tests.cpp
        commands/input_text_tests.cpp
        commands/invoke_method_tests.cpp
        commands/screenshot_base64_tests.cpp
        commands/set_property_tests.cpp
        commands/wait_for_item_tests.cpp
        commands/wait_tests.cpp
)

file(GLOB CORE_TEST_MOCK_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/mocks/*.cpp")
list(APPEND CORE_TEST_SOURCES ${CORE_TEST_MOCK_SOURCES})

foreach(f ${CORE_TEST_SOURCES})
    if(NOT EXISTS "${f}" AND NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${f}")
        message(FATAL_ERROR "Missing test file: ${f}")
    endif()
endforeach()

add_executable(WISKRCoreTests ${CORE_TEST_SOURCES})

target_include_directories(WISKRCoreTests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/wiskr
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(WISKRCoreTests
        PRIVATE
        WISKR::Core
        gtest::gtest

)



include(GoogleTest)
gtest_discover_tests(WISKRCoreTests)