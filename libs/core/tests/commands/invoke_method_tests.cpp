#include <gtest/gtest.h>
#include <wiskr/commands/invoke_method.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"

TEST(InvokeMethodTests, ReportsErrorWhenItemNotFound) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);

    whisker::ItemPath missing("nonexistent");
    std::promise<skitter::core::Value> promise;
    auto future = promise.get_future();
    whisker::cmd::InvokeMethod cmd(missing, "doSomething", {}, std::move(promise));
    cmd.execute(env);

    EXPECT_FALSE(env.state().errors().empty());
    auto status = future.wait_for(std::chrono::milliseconds(0));
    EXPECT_EQ(status, std::future_status::ready);
    EXPECT_NO_THROW({ future.get(); });
}

TEST(InvokeMethodTests, SetsPromiseDefaultAndDoesNotReportErrorWhenMethodInvokes) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem{}, path);
whisker::Environment env(scene, state);

std::promise<skitter::core::Value> promise;
auto future = promise.get_future();
whisker::cmd::InvokeMethod cmd(path, "existingMethod", {}, std::move(promise));
cmd.execute(env);

EXPECT_TRUE(env.state().errors().empty());
auto status = future.wait_for(std::chrono::milliseconds(0));
EXPECT_EQ(status, std::future_status::ready);
EXPECT_NO_THROW({ future.get(); });
}

TEST(InvokeMethodTests, PromiseIsReadyImmediatelyAfterExecute) {
whisker::MockScene scene;
whisker::State state;
whisker::ItemPath path("item1");
scene.addItemAtPath(whisker::MockItem{}, path);
whisker::Environment env(scene, state);

std::promise<skitter::core::Value> promise;
auto future = promise.get_future();
whisker::cmd::InvokeMethod cmd(path, "anyMethod", {}, std::move(promise));
cmd.execute(env);

auto status = future.wait_for(std::chrono::milliseconds(0));
EXPECT_EQ(status, std::future_status::ready);
}
