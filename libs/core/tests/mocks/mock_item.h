#ifndef WISKR_MOCK_ITEM_H
#define WISKR_MOCK_ITEM_H

#include <wiskr/scene/item.h>
#include <wiskr/data/geometry.h>
#include <skitr/core/value.h>
#include <string>
#include <vector>
#include <map>

namespace whisker {

    class MockItem: public Item {
    public:
        MockItem();
        MockItem(Size size);

        Size size() const override;
        Point position() const override;
        Rect bounds() const override;

        std::string stringProperty(const std::string& name) const override;
        void setStringProperty(const std::string &name, const std::string &value) override;

        bool invokeMethod(const std::string &name, const std::vector <skitter::core::Value> &args, skitter::core::Value &result) override;

        bool isVisible() const override;

        std::map <std::string, std::string> stringProperties() const;

    private:
        Size size_;
        std::map<std::string, std::string> stringProperties_;
    };
} // namespace whisker

#endif //WISKR_MOCK_ITEM_H
