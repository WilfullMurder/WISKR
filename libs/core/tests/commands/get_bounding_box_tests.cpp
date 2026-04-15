#include <gtest/gtest.h>
#include <wiskr/commands/get_bounding_box.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"

TEST(GetBoundingBoxTests, SetsPromiseToZeroAndReportsErrorWhenItemNotFound) {
whisker::MockScene scene;
whisker::State state;
whisker::Environment env(scene, state);

whisker::ItemPath missing("nonexistent");
std::promise<whisker::Rect> promise;
auto future = promise.get_future();
whisker::cmd::GetBoundingBox cmd(missing, std::move(promise));
cmd.execute(env);

EXPECT_TRUE(env.state().hasErrors());
auto rect = future.get();
EXPECT_DOUBLE_EQ(rect.topLeft.x, 0.0);
EXPECT_DOUBLE_EQ(rect.topLeft.y, 0.0);
EXPECT_DOUBLE_EQ(rect.size.width, 0.0);
EXPECT_DOUBLE_EQ(rect.size.height, 0.0);
}

TEST(GetBoundingBoxTests, SetsPromiseToItemBoundsWhenItemExists) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem{}, path);
whisker::Environment env(scene, state);

std::promise<whisker::Rect> promise;
auto future = promise.get_future();
whisker::cmd::GetBoundingBox cmd(path, std::move(promise));
cmd.execute(env);

EXPECT_FALSE(env.state().hasErrors());
auto rect = future.get();
// MockItem::bounds returns Rect(0.0, 0.0, 0.0, 0.0) for the mock; verify values are returned through the promise.
EXPECT_DOUBLE_EQ(rect.topLeft.x, 0.0);
EXPECT_DOUBLE_EQ(rect.topLeft.y, 0.0);
EXPECT_DOUBLE_EQ(rect.size.width, 0.0);
EXPECT_DOUBLE_EQ(rect.size.height, 0.0);
}

TEST(GetBoundingBoxTests, PromiseIsReadyImmediatelyAfterExecute) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem{}, path);
whisker::Environment env(scene, state);

std::promise<whisker::Rect> promise;
auto future = promise.get_future();
whisker::cmd::GetBoundingBox cmd(path, std::move(promise));
cmd.execute(env);

auto status = future.wait_for(std::chrono::milliseconds(0));
EXPECT_EQ(status, std::future_status::ready);
}
