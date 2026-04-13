#include <gtest/gtest.h>
#include <wiskr/commands/wait_for_item.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"


TEST(WaitForItemTests, ReturnsTrueWhenItemAlreadyExists) {
    whisker::MockScene scene;
    whisker::MockItem item;
    whisker::ItemPath path("item1");
    scene.addItemAtPath(item, path);
    whisker::State state;
    whisker::Environment env(scene, state);

    std::promise<bool> promise;
    auto future = promise.get_future();
    whisker::cmd::WaitForItem cmd(path, std::chrono::milliseconds(100), std::move(promise));

    EXPECT_TRUE(cmd.canExecute(env));
    cmd.execute(env);
    EXPECT_TRUE(future.wait_for(std::chrono::milliseconds(50)) == std::future_status::ready);
    EXPECT_TRUE(future.get());
}

TEST(WaitForItemTests, SetsPromiseWhenItemAppearsAfterExecute) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);
    whisker::ItemPath path("item2");

    std::promise<bool> promise;
    auto future = promise.get_future();
    whisker::cmd::WaitForItem cmd(path, std::chrono::milliseconds(200), std::move(promise));

    cmd.execute(env);
    EXPECT_FALSE(cmd.canExecute(env));

    whisker::MockItem item;
    scene.addItemAtPath(item, path);

    cmd.execute(env);
    EXPECT_TRUE(future.wait_for(std::chrono::milliseconds(200)) == std::future_status::ready);
    EXPECT_TRUE(future.get());
}

TEST(WaitForItemTests, BecomesReadyAfterTimeoutIfItemDoesNotAppear) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);
    whisker::ItemPath path("missing");

    std::promise<bool> promise;
    auto future = promise.get_future();
    whisker::cmd::WaitForItem cmd(path, std::chrono::milliseconds(50), std::move(promise));

    cmd.execute(env);
    EXPECT_FALSE(cmd.canExecute(env));

    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    EXPECT_TRUE(cmd.canExecute(env));
    EXPECT_TRUE(future.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout);
}

TEST(WaitForItemTests, ZeroTimeoutBecomesReadyImmediatelyAfterExecute) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);
    whisker::ItemPath path("zero");

    std::promise<bool> promise;
    auto future = promise.get_future();
    whisker::cmd::WaitForItem cmd(path, std::chrono::milliseconds(0), std::move(promise));

    cmd.execute(env);
    EXPECT_TRUE(cmd.canExecute(env));
    // promise should not be set because item is not present; still verify no blocking
    EXPECT_TRUE(future.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout);
}