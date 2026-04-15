#include "mock_item.h"
#include <skitr/core/value.h>

namespace whisker {

    MockItem::MockItem() {
        size_ = {0.0, 0.0};
    }

    MockItem::MockItem(Size size) : size_(size) {}

    Size MockItem::size() const {
        return size_;
    }

    Point MockItem::position() const {
        return {0.0, 0.0};
    }

    Rect MockItem::bounds() const {
        return Rect(0.0, 0.0, 0.0, 0.0);
    }

    std::string MockItem::stringProperty(const std::string &name) const {
        return stringProperties_.at(name);
    }

    void MockItem::setStringProperty(const std::string& name, const std::string& value) {
        stringProperties_[name] = value;
    }

    bool MockItem::invokeMethod(const std::string &name, const std::vector<skitter::core::Value> &args, skitter::core::Value &result) {
        return true;
    }

    bool MockItem::isVisible() const {
        return true;
    }

    std::map<std::string, std::string> MockItem::stringProperties() const {
        return stringProperties_;
    }


} // namespace whisker
