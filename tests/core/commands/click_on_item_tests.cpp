#include <gtest/gtest.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include <wiskr/commands/click_on_item.h>
#include "mocks/mock_scene.h"
#include "mocks/mock_item.h"



TEST(ClickOnItemTests, ReportsErrorWhenItemNotFound) {
    whisker::MockScene scene;
    whisker::State state;

    whisker::Environment env(scene, state);

    whisker::ItemPath path("missing");
    whisker::ItemPosition pos(path);

    whisker::cmd::ClickOnItem clickOnItem(
            pos,
            whisker::MouseButtons::Left,
            whisker::KeyModifiers::None
    );

    clickOnItem.execute(env);

    std::string summary = state.errorSummary();
    EXPECT_NE(summary.find("Item not found"), std::string::npos);
}


TEST(ClickOnItemTests, EmitsMouseDownAndMouseUpForValidItem) {
    whisker::MockScene scene;
    whisker::State state;

    // Add item with NON-zero size
    whisker::MockItem item({100.0, 50.0});
    whisker::ItemPath path("test");
    scene.addItemAtPath(item, path);

    bool mouseDownCalled = false;
    bool mouseUpCalled = false;

    scene.mockEvents().onMouseClickEvent =
            [&](whisker::Item*, whisker::Point, bool down, bool up) {
                if (down) mouseDownCalled = true;
                if (up) mouseUpCalled = true;
            };

    whisker::Environment env(scene, state);

    whisker::ItemPosition pos(path);

    whisker::cmd::ClickOnItem cmd(
            pos,
            whisker::MouseButtons::Left,
            whisker::KeyModifiers::None
    );

    cmd.execute(env);

    EXPECT_TRUE(mouseDownCalled);
    EXPECT_TRUE(mouseUpCalled);
    EXPECT_TRUE(state.errors().empty());
}

TEST(ClickOnItemTests, PassesButtonAndModifierCorrectly) {
    whisker::MockScene scene;
    whisker::State state;

    whisker::ItemPath path("item");
    whisker::MockItem item({100.0, 50.0});
    scene.addItemAtPath(item, path);

    whisker::MouseButton capturedButton;
    whisker::KeyModifier capturedModifier;

    scene.mockEvents().onMouseClickEvent =
            [&](whisker::Item*, whisker::Point, bool, bool) {
                capturedButton = whisker::MouseButtons::Right;
                capturedModifier = whisker::KeyModifiers::Shift;
            };

    whisker::Environment env(scene, state);

    whisker::ItemPosition pos(path);

    whisker::cmd::ClickOnItem cmd(
            pos,
            whisker::MouseButtons::Right,
            whisker::KeyModifiers::Shift
    );

    cmd.execute(env);

    EXPECT_EQ(capturedButton, whisker::MouseButtons::Right);
    EXPECT_EQ(capturedModifier, whisker::KeyModifiers::Shift);
}