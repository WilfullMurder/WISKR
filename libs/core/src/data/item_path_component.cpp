#include <wiskr/data/item_path_component.h>
#include <skitr/core/value.h>

namespace whisker {
    namespace path {

        NameSelector::NameSelector(std::string name) : name_(std::move(name)) {}

        const std::string& NameSelector::name() const {
            return name_;
        }

        PropertySelector::PropertySelector(std::string property) : property_(std::move(property)) {}

        const std::string& PropertySelector::property() const {
            return property_;
        }

        TypeSelector::TypeSelector(std::string type) : type_(std::move(type)) {}

        const std::string& TypeSelector::type() const {
            return type_;
        }

        ValueSelector::ValueSelector(std::string value) : value_(std::move(value)) {}

        const std::string& ValueSelector::value() const {
            return value_;
        }

        PropertyValueSelector::PropertyValueSelector(std::string property, std::string value)
            : property_(std::move(property)), value_(std::move(value)) {}

        const std::string& PropertyValueSelector::property() const {
            return property_;
        }

        const std::string& PropertyValueSelector::value() const {
            return value_;
        }

        Component::Component(const std::string& raw) {
            if(!raw.empty() && raw[0] == '.'){
                std::string property = raw.substr(1);
                selector_ = PropertySelector(std::move(property));
            }
            else if(!raw.empty() && raw[0] == '#') {
                std::string type = raw.substr(1);
                selector_ = TypeSelector(std::move(type));
            }
            else if(raw.size() >= 2 && raw[0] == '"' && raw[raw.size() - 1] == '"') {
                std::string value = raw.substr(1, raw.size() - 2);
                selector_ = ValueSelector(std::move(value));
            }
            else if(raw.size() >= 2 && raw[0] == '(' && raw[raw.size() - 1] == ')') {
                std::string content = raw.substr(1, raw.size() - 2);

                size_t eqPos = content.find('=');
                if (eqPos != std::string::npos) {
                    std::string property = content.substr(0, eqPos);
                    std::string value = content.substr(eqPos + 1);
                    selector_ = PropertyValueSelector(std::move(property), std::move(value));
                } else {
                    selector_ = NameSelector(raw);
                }
            }
            else {
                selector_ = NameSelector(raw);
            }
        }

        Component::Component(const Selector& selector) : selector_(selector) {}

        std::string Component::string() const {
            if(std::holds_alternative<NameSelector>(selector_)) {
                return std::get<NameSelector>(selector_).name();
            }
            else if(std::holds_alternative<PropertySelector>(selector_)) {
                return "." + std::get<PropertySelector>(selector_).property();
            }
            else if(std::holds_alternative<TypeSelector>(selector_)) {
                return "#" + std::get<TypeSelector>(selector_).type();
            }
            else if(std::holds_alternative<ValueSelector>(selector_)) {
                return "\"" + std::get<ValueSelector>(selector_).value() + "\"";
            }
            else if(std::holds_alternative<PropertyValueSelector>(selector_)) {
                const auto& pv = std::get<PropertyValueSelector>(selector_);
                return "(" + pv.property() + "=" + pv.value() + ")";
            }
            return "";
        }

        const Selector& Component::selector() const {
            return selector_;
        }

    } // namespace path
} // namespace whisker