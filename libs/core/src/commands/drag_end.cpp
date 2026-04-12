#include <wiskr/commands/drag_end.h>
#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        DragEnd::DragEnd(const ItemPath &itemPath) : itemPath_(itemPath) {}

        void DragEnd::execute(Environment &env) {
            auto item = env.scene().itemAt(itemPath_);
            if (!item) {
                env.state().reportError("DragEnd: Item not found at path: " + itemPath_.string());
                return;
            }
            auto size = item->size();
            Point midPoint(size.width / 2.0, size.height / 2.0);
            env.scene().events().mouseMove(item.get(), midPoint);
            env.scene().events().mouseUp(item.get(), midPoint, MouseButtons::Left, KeyModifiers::None);
        }
    } // namespace cmd
} // namespace whisker