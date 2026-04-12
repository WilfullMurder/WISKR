#include <wiskr/commands/get_property.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        GetProperty::GetProperty(const ItemPath &targetPath, const std::string &propertyName, std::promise<std::string> promise) : targetPath_(targetPath), propertyName_(propertyName), promise_(std::move(promise)) {}

        void GetProperty::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);

            if(!item){
                promise_.set_value("");
                env.state().reportError("GetProperty: Item not found at path: " + targetPath_.string());
                return;
            }

            auto value = item->stringProperty(propertyName_);
            promise_.set_value(value);
        }
    } // namespace cmd
} // namespace whisker