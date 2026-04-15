#include <gtest/gtest.h>
#include <wiskr/commands/enter_key.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"

TEST(EnterKeyTests, ReportsErrorWhenItemNotFound) {
whisker::MockScene scene;
whisker::State state;
whisker::Environment env(scene, state);

whisker::ItemPath missing("nonexistent");
whisker::cmd::EnterKey cmd(missing, 13, static_cast<whisker::KeyModifier>(0));
cmd.execute(env);

EXPECT_FALSE(env.state().errors().empty());
}

TEST(EnterKeyTests, DoesNotReportErrorWhenItemFound) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem{}, path);
whisker::Environment env(scene, state);

whisker::cmd::EnterKey cmd(path, 13, static_cast<whisker::KeyModifier>(0));
cmd.execute(env);

EXPECT_TRUE(env.state().errors().empty());
}

TEST(EnterKeyTests, HandlesZeroKeyCodeGracefully) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem{}, path);
whisker::Environment env(scene, state);

whisker::cmd::EnterKey cmd(path, 0, static_cast<whisker::KeyModifier>(0));
cmd.execute(env);

EXPECT_TRUE(env.state().errors().empty());
}