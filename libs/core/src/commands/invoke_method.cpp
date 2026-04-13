#include <wiskr/commands/invoke_method.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        InvokeMethod::InvokeMethod(const ItemPath &targetPath, const std::string &methodName, const std::vector<skitter::core::Value> &args, std::promise<skitter::core::Value> promise)
                : targetPath_(targetPath), methodName_(methodName), args_(args), promise_(std::move(promise)) {}

        void InvokeMethod::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);

            if(!item){
                env.state().reportError("InvokeMethod: Item not found at path: " + targetPath_.string());
                promise_.set_value(skitter::core::Value());
                return;
            }
            skitter::core::Value ret;
            if(!item->invokeMethod(methodName_, args_, ret)){
                env.state().reportError("InvokeMethod: Method '" + methodName_ + "' not found or failed on item at path: " + targetPath_.string());
                promise_.set_value(skitter::core::Value());
                return;
            }
            promise_.set_value(std::move(ret));
        }
    } // namespace cmd
} // namespace whisker