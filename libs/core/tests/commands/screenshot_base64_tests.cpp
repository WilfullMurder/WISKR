#include <gtest/gtest.h>
#include <wiskr/commands/screenshot_base64.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"


TEST(ScreenshotBase64Tests, ReportsErrorWhenItemNotFoundAndSetsEmptyPromise) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);

    whisker::ItemPath missing("nonexistent");
    std::promise<std::string> promise;
    auto future = promise.get_future();
    whisker::cmd::ScreenshotBase64 cmd(missing, std::move(promise));
    cmd.execute(env);

    EXPECT_FALSE(env.state().errors().empty());
    auto status = future.wait_for(std::chrono::milliseconds(0));
    EXPECT_EQ(status, std::future_status::ready);
    EXPECT_EQ(future.get(), "");
}

TEST(ScreenshotBase64Tests, ReturnsBase64StringWhenItemExists) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::ItemPath path("item1");
    scene.addItemAtPath(whisker::MockItem{}, path);
    whisker::Environment env(scene, state);

    std::promise<std::string> promise;
    auto future = promise.get_future();
    whisker::cmd::ScreenshotBase64 cmd(path, std::move(promise));
    cmd.execute(env);

    EXPECT_TRUE(env.state().errors().empty());
    auto status = future.wait_for(std::chrono::milliseconds(0));
    EXPECT_EQ(status, std::future_status::ready);
    EXPECT_EQ(future.get(), "Base64 String");
}

TEST(ScreenshotBase64Tests, PromiseIsReadyImmediatelyAfterExecuteWhenItemExists) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::ItemPath path("item1");
    scene.addItemAtPath(whisker::MockItem{}, path);
    whisker::Environment env(scene, state);

    std::promise<std::string> promise;
    auto future = promise.get_future();
    whisker::cmd::ScreenshotBase64 cmd(path, std::move(promise));
    cmd.execute(env);

    auto status = future.wait_for(std::chrono::milliseconds(0));
    EXPECT_EQ(status, std::future_status::ready);
}
