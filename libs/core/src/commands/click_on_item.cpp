#include <wiskr/commands/click_on_item.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        ClickOnItem::ClickOnItem(const ItemPosition& position, MouseButton button, KeyModifier keyModifier)
            : position_(position), button_(button), keyModifier_(keyModifier) {}

        void ClickOnItem::execute(Environment& env) {
            auto path = position_.itemPath();
            auto item = env.scene().itemAt(path);

            if(!item) {
                env.state().reportError("ClickOnItem: Item not found at path: " + path.string());
                return;
            }

            auto size = item->size();
            auto mousePoint = position_.positionForItemSize(size);
            env.scene().events().mouseDown(item.get(), mousePoint, button_, keyModifier_);
            env.scene().events().mouseUp(item.get(), mousePoint, button_, keyModifier_);
        }

    } // namespace cmd
} // namespace whisker
