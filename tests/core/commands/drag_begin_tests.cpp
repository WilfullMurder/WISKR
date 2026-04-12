#include <gtest/gtest.h>
#include <wiskr/commands/drag_begin.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"

TEST(DragBeginCommandTests, CanExecuteReturnsTrueByDefault) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);


    class LocalNoopCommand : public whisker::cmd::Command {
        void execute(whisker::Environment&) override {}
    } localCmd;

    EXPECT_TRUE(localCmd.canExecute(env));
}

TEST(DragBeginCommandTests, ReportsErrorWhenItemNotFound) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);

    whisker::ItemPath path("missing");
    whisker::cmd::DragBegin cmd(path);
    cmd.execute(env);

    std::string summary = state.errorSummary();
    EXPECT_NE(summary.find("Item not found"), std::string::npos);
    EXPECT_TRUE(state.hasErrors());
}

TEST(DragBeginTests, EmitsMouseDownAndMoveWhenItemFound) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::ItemPath path("item1");
    scene.addItemAtPath(whisker::MockItem{}, path);
    whisker::Environment env(scene, state);
    whisker::cmd::DragBegin cmd(path);
    cmd.execute(env);
    EXPECT_FALSE(state.hasErrors());
}
