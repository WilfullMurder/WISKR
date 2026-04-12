#include <wiskr/commands/drag_begin.h>
#include <wiskr/scene/scene.h>
#include <wiskr/events/identifiers.h>

namespace whisker {
    namespace cmd {

        DragBegin::DragBegin(const ItemPath &itemPath) : itemPath_(itemPath) {}

        void DragBegin::execute(Environment &env) {
            auto item = env.scene().itemAt(itemPath_);
            if (!item) {
                env.state().reportError("DragBegin: Item not found at path: " + itemPath_.string());
                return;
            }
            auto size = item->size();
            Point midPoint(size.width / 2.0, size.height / 2.0);
            env.scene().events().mouseDown(item.get(), midPoint, MouseButtons::Left, KeyModifiers::None);
            env.scene().events().mouseMove(item.get(), midPoint);
        }
    } // namespace cmd
} // namespace whisker

