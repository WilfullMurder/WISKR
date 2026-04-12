#include <wiskr/commands/input_text.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        InputText::InputText(const ItemPath &targetPath, const std::string &text) : targetPath_(targetPath), text_(text) {}

        void InputText::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);

            if(!item){
                env.state().reportError("InputText: Item not found at path: " + targetPath_.string());
                return;
            }
            env.scene().events().stringInput(item.get(), text_);
        }
    } // namespace cmd
} // namespace whisker