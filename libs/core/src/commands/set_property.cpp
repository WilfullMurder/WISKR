#include <wiskr/commands/set_property.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        SetProperty::SetProperty(const ItemPath &targetPath, const std::string &propertyName, const std::string &value) : targetPath_(targetPath), propertyName_(propertyName), value_(value) {}

        void SetProperty::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);

            if(!item){
                env.state().reportError("SetProperty: Item not found at path: " + targetPath_.string());
                return;
            }
            item->setStringProperty(propertyName_, value_);
        }
    } // namespace cmd
} // namespace whisker