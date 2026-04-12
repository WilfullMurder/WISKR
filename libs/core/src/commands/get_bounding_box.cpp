#include <wiskr/commands/get_bounding_box.h>

#include <wiskr/scene/scene.h>

namespace whisker{
    namespace cmd {

        GetBoundingBox::GetBoundingBox(const ItemPath &targetPath, std::promise<Rect> promise) : targetPath_(targetPath), promise_(std::move(promise)) {}

        void GetBoundingBox::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);
            if (!item) {
                env.state().reportError("GetBoundingBox: Item not found at path: " + targetPath_.string());
                promise_.set_value(Rect{0, 0, 0, 0});
                return;
            }
            promise_.set_value(item->bounds());
        }
    } // namespace cmd
} // namespace whisker