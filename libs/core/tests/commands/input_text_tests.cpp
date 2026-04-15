#include <gtest/gtest.h>
#include <wiskr/commands/input_text.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"


TEST(InputTextTests, ReportsErrorWhenItemNotFound) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::Environment env(scene, state);

    whisker::ItemPath missing("nonexistent");
    whisker::cmd::InputText cmd(missing, "hello");
    cmd.execute(env);

    EXPECT_FALSE(env.state().errors().empty());
}

TEST(InputTextTests, EmitsStringInputEventWhenItemExists) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::ItemPath path("item1");
    scene.addItemAtPath(whisker::MockItem{}, path);

    whisker::Environment env(scene, state);

    bool called = false;
    std::string received;
    scene.mockEvents().onStringInputEvent = [&](whisker::Item* /*item*/, const std::string& text){
        called = true; received = text;
    };

    whisker::cmd::InputText cmd(path, "hello");
    cmd.execute(env);

    EXPECT_TRUE(env.state().errors().empty());
    EXPECT_TRUE(called);
    EXPECT_EQ(received, "hello");
}

TEST(InputTextTests, EmitsStringInputEventWithEmptyText) {
    whisker::MockScene scene;
    whisker::State state;
    whisker::ItemPath path("item1");
    scene.addItemAtPath(whisker::MockItem{}, path);

    whisker::Environment env(scene, state);

    bool called = false;
    std::string received = "unset";
    scene.mockEvents().onStringInputEvent = [&](whisker::Item* /*item*/, const std::string& text){
        called = true; received = text;
    };

    whisker::cmd::InputText cmd(path, "");
    cmd.execute(env);

    EXPECT_TRUE(env.state().errors().empty());
    EXPECT_TRUE(called);
    EXPECT_EQ(received, "");
}
