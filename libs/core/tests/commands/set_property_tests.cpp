#include <gtest/gtest.h>
#include <wiskr/commands/set_property.h>
#include <wiskr/dispatch/environment.h>
#include <wiskr/dispatch/state.h>
#include "mocks/mock_scene.h"

TEST(SetPropertyTests, SetsStringPropertyWhenItemExists) {
    struct CapturingScene : whisker::MockScene {
        mutable whisker::MockItem stored;
        struct ProxyItem : public whisker::MockItem {
            whisker::MockItem* ref;
            ProxyItem(whisker::MockItem* r) : ref(r) {}
            std::string stringProperty(const std::string &name) const override {
                return ref->stringProperty(name);
            }
            void setStringProperty(const std::string& name, const std::string& value) override {
                ref->setStringProperty(name, value);
            }
        };
        std::unique_ptr<whisker::Item> itemAt(const whisker::ItemPath& /*path*/) const override {
            return std::make_unique<ProxyItem>(&stored);
        }
    };

    CapturingScene scene;
    whisker::State state;
    whisker::ItemPath path("item1");
    whisker::Environment env(scene, state);

    whisker::cmd::SetProperty cmd(path, "prop", "value");
    cmd.execute(env);

    EXPECT_TRUE(env.state().errors().empty());
    EXPECT_EQ(scene.stored.stringProperty("prop"), "value");
}

TEST(SetPropertyTests, OverwritesExistingPropertyWhenItemExists) {
    struct CapturingScene : whisker::MockScene {
        mutable whisker::MockItem stored;
        struct ProxyItem : public whisker::MockItem {
            whisker::MockItem* ref;
            ProxyItem(whisker::MockItem* r) : ref(r) {}
            std::string stringProperty(const std::string &name) const override {
                return ref->stringProperty(name);
            }
            void setStringProperty(const std::string& name, const std::string& value) override {
                ref->setStringProperty(name, value);
            }
        };
        std::unique_ptr<whisker::Item> itemAt(const whisker::ItemPath& /*path*/) const override {
            return std::make_unique<ProxyItem>(&stored);
        }
    };

    CapturingScene scene;
    scene.stored.setStringProperty("prop", "old");
    whisker::State state;
    whisker::ItemPath path("item1");
    whisker::Environment env(scene, state);

    whisker::cmd::SetProperty cmd(path, "prop", "new");
    cmd.execute(env);

    EXPECT_TRUE(env.state().errors().empty());
    EXPECT_EQ(scene.stored.stringProperty("prop"), "new");
}

TEST(SetPropertyTests, AllowsEmptyPropertyNameAndSetsValue) {
    struct CapturingScene : whisker::MockScene {
        mutable whisker::MockItem stored;
        struct ProxyItem : public whisker::MockItem {
            whisker::MockItem* ref;
            ProxyItem(whisker::MockItem* r) : ref(r) {}
            std::string stringProperty(const std::string &name) const override {
                return ref->stringProperty(name);
            }
            void setStringProperty(const std::string& name, const std::string& value) override {
                ref->setStringProperty(name, value);
            }
        };
        std::unique_ptr<whisker::Item> itemAt(const whisker::ItemPath& /*path*/) const override {
            return std::make_unique<ProxyItem>(&stored);
        }
    };

    CapturingScene scene;
    whisker::State state;
    whisker::ItemPath path("item1");
    whisker::Environment env(scene, state);

    whisker::cmd::SetProperty cmd(path, "", "emptyNameValue");
    cmd.execute(env);

    EXPECT_TRUE(env.state().errors().empty());
    EXPECT_EQ(scene.stored.stringProperty(""), "emptyNameValue");
}