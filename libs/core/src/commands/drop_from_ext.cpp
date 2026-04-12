#include <wiskr/commands/drop_from_ext.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        DropFromExt::DropFromExt(const ItemPath &targetPath, const PasteboardContent &content) : targetPath_(targetPath), content_(content) {}

        void DropFromExt::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);
            if (!item) {
                env.state().reportError("DropFromExt: Item not found at path: " + targetPath_.string());
                return;
            }
            auto size = item->size();
            Point midPoint(size.width / 2.0, size.height / 2.0);
            env.scene().events().extMouseDrop(item.get(), midPoint, content_);
        }
    } // namespace cmd
} // namespace whisker