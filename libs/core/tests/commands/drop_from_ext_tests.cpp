#include <gtest/gtest.h>
#include <wiskr/commands/drop_from_ext.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"


TEST(DropFromExtCommandTests, ReportsErrorWhenItemNotFound) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);

    whisker::ItemPath path("missing");
    whisker::PasteboardContent content;
    whisker::cmd::DropFromExt cmd(path, content);
    cmd.execute(env);

    std::string summary = state.errorSummary();
    EXPECT_NE(summary.find("Item not found"), std::string::npos);
    EXPECT_TRUE(state.hasErrors());
}

TEST(DropFromExtTests, EmitsExtMouseDropAtMidpointForItemWithSize) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem({100.0, 60.0}), path);

whisker::Environment env(scene, state);

bool called = false;
whisker::Point receivedLoc{};
whisker::Item* receivedItem = nullptr;
scene.mockEvents().onExtMouseDropEvent = [&](whisker::Item* item, whisker::Point loc, whisker::PasteboardContent&){
    called = true; receivedItem = item; receivedLoc = loc;
};

whisker::PasteboardContent content;
whisker::cmd::DropFromExt cmd(path, content);
cmd.execute(env);

EXPECT_TRUE(env.state().errors().empty());
EXPECT_TRUE(called);
EXPECT_NE(receivedItem, nullptr);
EXPECT_DOUBLE_EQ(receivedLoc.x, 50.0);
EXPECT_DOUBLE_EQ(receivedLoc.y, 30.0);
}

TEST(DropFromExtTests, EmitsExtMouseDropAtZeroForZeroSizeItem) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("zero_item");
scene.addItemAtPath(whisker::MockItem{}, path);

whisker::Environment env(scene, state);

bool called = false;
whisker::Point receivedLoc{};
scene.mockEvents().onExtMouseDropEvent = [&](whisker::Item* /*item*/, whisker::Point loc, whisker::PasteboardContent&){
    called = true; receivedLoc = loc;
};

whisker::PasteboardContent content;
whisker::cmd::DropFromExt cmd(path, content);
cmd.execute(env);

EXPECT_TRUE(env.state().errors().empty());
EXPECT_TRUE(called);
EXPECT_DOUBLE_EQ(receivedLoc.x, 0.0);
EXPECT_DOUBLE_EQ(receivedLoc.y, 0.0);
}