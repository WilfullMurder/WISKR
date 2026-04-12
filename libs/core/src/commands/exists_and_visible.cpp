#include <wiskr/commands/exists_and_visible.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        ExistsAndVisible::ExistsAndVisible(const ItemPath &targetPath, std::promise<bool> promise) : targetPath_(targetPath), promise_(std::move(promise)) {}

        void ExistsAndVisible::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);
            if(!item) {
                promise_.set_value(false);
                return;
            }

            promise_.set_value(item->isVisible());
        }
    } // namespace cmd
} // namespace whisker