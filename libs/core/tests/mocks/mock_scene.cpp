#include "mock_scene.h"
#include "mock_item.h"

namespace whisker {

    std::unique_ptr<Item> MockScene::itemAt(const ItemPath& path) const {
        auto found = items_.find(path.string());
        if (found != items_.end()) {
            return std::make_unique<MockItem>(found->second);
        }
        return nullptr;
    }

    Events& MockScene::events() {
        return events_;
    }

    void MockScene::takeScreenshot(const ItemPath& /*targetItem*/, const std::string& /*filename*/) {
        // no op for mock
    }

    std::string MockScene::takeScreenshotBase64(const ItemPath& /*targetItem*/) {
        return "Base64 String";
    }

    void MockScene::addItemAtPath(MockItem item, const ItemPath& path) {
        items_.emplace(std::make_pair(path.string(), std::move(item)));
    }

    MockEvents& MockScene::mockEvents() {
        return events_;
    }

} // namespace whisker

