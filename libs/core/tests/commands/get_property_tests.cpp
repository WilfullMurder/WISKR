#include <gtest/gtest.h>
#include <wiskr/commands/get_property.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"
#include "mocks/mock_item.h"

TEST(GetPropertyTests, SetsPromiseEmptyAndReportsErrorWhenItemNotFound) {
whisker::MockScene scene;
whisker::State state;
whisker::Environment env(scene, state);

whisker::ItemPath missing("nonexistent");
std::promise<std::string> promise;
auto future = promise.get_future();
whisker::cmd::GetProperty cmd(missing, "any", std::move(promise));
cmd.execute(env);

EXPECT_FALSE(env.state().errors().empty());
EXPECT_EQ(future.get(), "");
}

TEST(GetPropertyTests, SetsPromiseToPropertyValueWhenItemExists) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");

whisker::MockItem item;
item.setStringProperty("label", "hello");
scene.addItemAtPath(std::move(item), path);

whisker::Environment env(scene, state);

std::promise<std::string> promise;
auto future = promise.get_future();
whisker::cmd::GetProperty cmd(path, "label", std::move(promise));
cmd.execute(env);

EXPECT_TRUE(env.state().errors().empty());
EXPECT_EQ(future.get(), "hello");
}

TEST(GetPropertyTests, PromiseIsReadyImmediatelyAfterExecute) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");

whisker::MockItem item;
item.setStringProperty("label", "ready");
scene.addItemAtPath(std::move(item), path);

whisker::Environment env(scene, state);

std::promise<std::string> promise;
auto future = promise.get_future();
whisker::cmd::GetProperty cmd(path, "label", std::move(promise));
cmd.execute(env);

auto status = future.wait_for(std::chrono::milliseconds(0));
EXPECT_EQ(status, std::future_status::ready);
}