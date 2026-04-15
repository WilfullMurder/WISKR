#include <gtest/gtest.h>
#include <wiskr/commands/wait.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"


TEST(WaitTests, ReturnsFalseInitiallyAndTrueAfterDuration) {
whisker::MockScene scene;
whisker::State state;
whisker::Environment env(scene, state);

whisker::cmd::Wait cmd(std::chrono::milliseconds(50));
EXPECT_FALSE(cmd.canExecute(env));
std::this_thread::sleep_for(std::chrono::milliseconds(60));
EXPECT_TRUE(cmd.canExecute(env));
}

TEST(WaitTests, MultipleCanExecuteCallsBeforeElapsedRemainFalse) {
whisker::MockScene scene;
whisker::State state;
whisker::Environment env(scene, state);

whisker::cmd::Wait cmd(std::chrono::milliseconds(200));
EXPECT_FALSE(cmd.canExecute(env));
EXPECT_FALSE(cmd.canExecute(env));
std::this_thread::sleep_for(std::chrono::milliseconds(210));
EXPECT_TRUE(cmd.canExecute(env));
}

TEST(WaitTests, ZeroDurationBecomesReadyOnSecondCall) {
whisker::MockScene scene;
whisker::State state;
whisker::Environment env(scene, state);

whisker::cmd::Wait cmd(std::chrono::milliseconds(0));
EXPECT_FALSE(cmd.canExecute(env));
EXPECT_TRUE(cmd.canExecute(env));
}

TEST(WaitTests, ExecuteDoesNotStartTimer) {
whisker::MockScene scene;
whisker::State state;
whisker::Environment env(scene, state);

whisker::cmd::Wait cmd(std::chrono::milliseconds(50));
cmd.execute(env);
EXPECT_FALSE(cmd.canExecute(env));
std::this_thread::sleep_for(std::chrono::milliseconds(60));
EXPECT_TRUE(cmd.canExecute(env));
}
