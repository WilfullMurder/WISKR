#include <wiskr/commands/enter_key.h>
#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        EnterKey::EnterKey(const ItemPath &targetPath, int keyCode, KeyModifier mod) : targetPath_(targetPath), keyCode_(keyCode), mod_(mod) {}

        void EnterKey::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);
            if (!item) {
                env.state().reportError("EnterKey: Item not found at path: " + targetPath_.string());
                return;
            }
            env.scene().events().keyPress(item.get(), keyCode_, mod_);
            env.scene().events().keyRelease(item.get(), keyCode_, mod_);
        }
    } // namespace cmd
} // namespace whisker