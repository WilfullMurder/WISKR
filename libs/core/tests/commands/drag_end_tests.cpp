#include <gtest/gtest.h>
#include <wiskr/commands/drag_end.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include <wiskr/data/item_path.h>
#include "mocks/mock_scene.h"

TEST(DragEndCommandTests, ReportsErrorWhenItemNotFound) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);

    whisker::ItemPath path("missing");
    whisker::cmd::DragEnd cmd(path);
    cmd.execute(env);

    std::string summary = state.errorSummary();
    EXPECT_NE(summary.find("Item not found"), std::string::npos);
    EXPECT_TRUE(state.hasErrors());
}

TEST(DragEndTests, EmitsMouseMoveThenMouseUpAtMidpointWhenItemFound) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::ItemPath path("item1");
    scene.addItemAtPath(whisker::MockItem({100.0, 60.0}), path);

    whisker::Environment env(scene, state);

    std::vector<std::string> sequence;
    std::vector<whisker::Point> points;
    scene.mockEvents().onMouseClickEvent = [&](whisker::Item* item, whisker::Point loc, bool clicked, bool released) {
        if (!clicked && !released) { sequence.push_back("move"); points.push_back(loc); }
        if (!clicked && released) { sequence.push_back("up"); points.push_back(loc); }
    };

    whisker::cmd::DragEnd cmd(path);
    cmd.execute(env);

    EXPECT_TRUE(env.state().errors().empty());
    ASSERT_GE(sequence.size(), 2u);
    EXPECT_EQ(sequence[0], "move");
    EXPECT_EQ(sequence[1], "up");

    EXPECT_DOUBLE_EQ(points[0].x, 50.0);
    EXPECT_DOUBLE_EQ(points[0].y, 30.0);
    EXPECT_DOUBLE_EQ(points[1].x, 50.0);
    EXPECT_DOUBLE_EQ(points[1].y, 30.0);
}

TEST(DragEndTests, EmitsEventsForZeroSizeItem) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::ItemPath path("zero_item");
    scene.addItemAtPath(whisker::MockItem{}, path);

    whisker::Environment env(scene, state);

    std::vector<std::string> sequence;
    std::vector<whisker::Point> points;
    scene.mockEvents().onMouseClickEvent = [&](whisker::Item* item, whisker::Point loc, bool clicked, bool released) {
        if (!clicked && !released) { sequence.push_back("move"); points.push_back(loc); }
        if (!clicked && released) { sequence.push_back("up"); points.push_back(loc); }
    };

    whisker::cmd::DragEnd cmd(path);
    cmd.execute(env);

    EXPECT_TRUE(env.state().errors().empty());
    ASSERT_GE(sequence.size(), 2u);
    EXPECT_EQ(sequence[0], "move");
    EXPECT_EQ(sequence[1], "up");

    EXPECT_DOUBLE_EQ(points[0].x, 0.0);
    EXPECT_DOUBLE_EQ(points[0].y, 0.0);
    EXPECT_DOUBLE_EQ(points[1].x, 0.0);
    EXPECT_DOUBLE_EQ(points[1].y, 0.0);
}