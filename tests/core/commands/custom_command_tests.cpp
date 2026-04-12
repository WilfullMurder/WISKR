#include <gtest/gtest.h>
#include <wiskr/dispatch/state.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/commands/custom_command.h>
#include "mocks/mock_scene.h"



TEST(CustomCommandTests, ExecutesProvidedActionWhenExecuted) {
    int called = 0;
    whisker::MockScene scene;
    whisker::State state;

    whisker::Environment env(scene, state);
    std::function<void(whisker::Environment&)> action = [&](whisker::Environment&){ ++called; };
    std::function<bool()> can_exec = [](){ return true; };
    whisker::cmd::CustomCommand cmd(action, can_exec);
    cmd.execute(env);
    EXPECT_EQ(called, 1);
}

TEST(CustomCommandTests, CanExecReturnsTrueByDefault) {
    whisker::MockScene scene;
    whisker::State state;

    whisker::Environment env(scene, state);
    std::function<void(whisker::Environment&)> no_op = [](whisker::Environment&){};
    std::function<bool()> can_exec = [](){ return true; };
    whisker::cmd::CustomCommand cmd(no_op, can_exec);
    EXPECT_TRUE(cmd.canExecute(env));
}