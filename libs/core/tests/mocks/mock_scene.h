#ifndef WISKR_MOCK_SCENE_H
#define WISKR_MOCK_SCENE_H

#include <wiskr/scene/scene.h>
#include "mock_item.h"
#include "mock_events.h"
#include <map>
#include <string>
#include <memory>

namespace whisker {

    class MockScene: public Scene {
    public:
        ~MockScene() override = default;

        std::unique_ptr<Item> itemAt(const ItemPath& path) const override;

        Events& events() override;

        void takeScreenshot(const ItemPath& targetItem, const std::string& filename) override;
        std::string takeScreenshotBase64(const ItemPath& targetItem) override;

        void addItemAtPath(MockItem item, const ItemPath& path);
        MockEvents& mockEvents();

    private:
        std::map<std::string, MockItem> items_;
        MockEvents events_;
    };

} // namespace whisker

#endif //WISKR_MOCK_SCENE_H
