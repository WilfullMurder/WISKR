#include <wiskr/commands/screenshot.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        Screenshot::Screenshot(const ItemPath &targetPath, std::string filePath) : targetPath_(targetPath), filePath_(std::move(filePath)) {}

        void Screenshot::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);

            if(!item){
                env.state().reportError("Screenshot: Item not found at path: " + targetPath_.string());
                return;
            }
            env.scene().takeScreenshot(targetPath_, filePath_);
        }
    } // namespace cmd
} // namespace whisker