#include <wiskr/data/item_position.h>

namespace whisker {

    ItemPosition::ItemPosition(ItemPath path, Point proportion, Point offset)
        : path_(std::move(path)), proportion_(proportion), offset_(offset) {}

    const ItemPath& ItemPosition::itemPath() const {
        return path_;
    }

    Point ItemPosition::positionForItemSize(const Size& size) const {
        return {size.width * proportion_.x + offset_.x, size.height * proportion_.y + offset_.y};
    }

} // namespace whisker