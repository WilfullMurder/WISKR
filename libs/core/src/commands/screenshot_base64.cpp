#include <wiskr/commands/screenshot_base64.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        ScreenshotBase64::ScreenshotBase64(const ItemPath &targetPath, std::promise<std::string> promise) : targetPath_(targetPath), promise_(std::move(promise)) {}

        void ScreenshotBase64::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);

            if(!item){
                env.state().reportError("ScreenshotBase64: Item not found at path: " + targetPath_.string());
                promise_.set_value("");
                return;
            }
            std::string base64 = env.scene().takeScreenshotBase64(targetPath_);
            promise_.set_value(std::move(base64));
        }
    } // namespace cmd
} // namespace whisker