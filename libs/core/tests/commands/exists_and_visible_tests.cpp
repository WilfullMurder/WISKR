#include <gtest/gtest.h>
#include <wiskr/commands/exists_and_visible.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"

TEST(ExistsAndVisibleTests, SetsPromiseFalseWhenItemNotFound) {
whisker::MockScene scene;
whisker::State state;
whisker::Environment env(scene, state);

whisker::ItemPath missing("nonexistent");
std::promise<bool> promise;
auto future = promise.get_future();
whisker::cmd::ExistsAndVisible cmd(missing, std::move(promise));
cmd.execute(env);

EXPECT_FALSE(future.get());
}

TEST(ExistsAndVisibleTests, SetsPromiseTrueWhenItemExistsAndVisible) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem{}, path);
whisker::Environment env(scene, state);

std::promise<bool> promise;
auto future = promise.get_future();
whisker::cmd::ExistsAndVisible cmd(path, std::move(promise));
cmd.execute(env);

EXPECT_TRUE(future.get());
}

TEST(ExistsAndVisibleTests, PromiseIsReadyImmediatelyAfterExecute) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem{}, path);
whisker::Environment env(scene, state);

std::promise<bool> promise;
auto future = promise.get_future();
whisker::cmd::ExistsAndVisible cmd(path, std::move(promise));
cmd.execute(env);

auto status = future.wait_for(std::chrono::milliseconds(0));
EXPECT_EQ(status, std::future_status::ready);
}

