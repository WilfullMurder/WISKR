#include <gtest/gtest.h>
#include <wiskr/dispatch/dispatcher.h>
#include <wiskr/commands/command.h>
#include <wiskr/dispatch/state.h>
#include <wiskr/dispatch/environment.h>
#include <memory>

using namespace whisker;

// Provide a concrete Scene type for the tests (Scenes are optional in the project).
namespace whisker { struct Scene { int value = 0; }; }

struct IncrementCommand : cmd::Command {
    int delta;
    Scene* scene;
    IncrementCommand(int d, Scene* s) : delta(d), scene(s) {}
    void execute(Environment& env) override { scene->value += delta; }
    bool canExecute(Environment& env) const override { return false; }
};

struct FlagCommand : cmd::Command {
    bool* flag;
    FlagCommand(bool* f) : flag(f) {}
    void execute(Environment& env) override { *flag = true; }
    bool canExecute(Environment& env) const override { return false; }
};

TEST(DispatcherExecutesSingleCommandWhenCanExecuteIsFalse, increments_scene_value_by_command_delta) {
    Dispatcher dispatcher;
    Scene scene;
    dispatcher.enqueueCommand(std::make_unique<IncrementCommand>(5, &scene));
    dispatcher.processCommands(scene);
    EXPECT_EQ(scene.value, 5);
}

TEST(DispatcherProcessesMultipleCommandsInFIFOOrderWhenAllCanExecuteAreFalse, accumulates_scene_value_in_order) {
    Dispatcher dispatcher;
    Scene scene;
    dispatcher.enqueueCommand(std::make_unique<IncrementCommand>(2, &scene));
    dispatcher.enqueueCommand(std::make_unique<IncrementCommand>(3, &scene));
    dispatcher.processCommands(scene);
    EXPECT_EQ(scene.value, 5);
}

TEST(DispatcherDoesNotExecuteCommandWhenCanExecuteReturnsTrue, leaves_command_unexecuted) {
    Dispatcher dispatcher;
    Scene scene;
    bool executed = false;
    struct BlockingCmd : cmd::Command {
        bool* out;
        BlockingCmd(bool* o) : out(o) {}
        void execute(Environment& env) override { *out = true; }
        bool canExecute(Environment& env) const override { return true; }
    };
    dispatcher.enqueueCommand(std::make_unique<BlockingCmd>(&executed));
    dispatcher.processCommands(scene);
    EXPECT_FALSE(executed);
}

TEST(EnvironmentProvidesReferencesToSameStateAndSceneInstances, env_state_and_scene_are_references_to_originals) {
    State s;
    Scene scene;
    Environment env(scene, s);
    env.state().reportError("an error");
    EXPECT_TRUE(env.state().hasErrors());
    EXPECT_EQ(static_cast<const void*>(&env.scene()), static_cast<const void*>(&scene));
    EXPECT_EQ(env.state().errorSummary(), std::string("an error"));
}