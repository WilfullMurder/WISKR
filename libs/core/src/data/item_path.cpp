#include <wiskr/data/item_path.h>

#include <wiskr/utils/path_parser.h>

#include <iterator>

namespace whisker{

    ItemPath::ItemPath() = default;

    ItemPath::ItemPath(const char* path) : ItemPath(std::string(path)) {}

    ItemPath::ItemPath(const std::string& path) {
        auto stringComponents = utils::ParsePathFrom(path);
        for (const auto& component : stringComponents) {
            components_.emplace_back(path::Component(component));
        }
    }

    ItemPath::ItemPath(const std::vector<path::Component>& components) : components_(components) {}

    const std::vector<path::Component>& ItemPath::components() const {
        return components_;
    }

    const path::Component& ItemPath::root() const {
        return components_.at(0);
    }

    std::string ItemPath::string() const {
        std::vector<std::string> componentStrings;
        for (const auto& component : components_) {
            componentStrings.emplace_back(component.string());
        }
        return utils::FormatPath(componentStrings);
    }

        ItemPath ItemPath::subPath(size_t offset) const {
        if (offset >= components_.size()) {
            return ItemPath();
        }
        std::vector<path::Component> subComponents;
        std::copy(components_.begin() + offset, components_.end(), std::back_inserter(subComponents));
        return ItemPath(std::move(subComponents));
    }
}// namespace whisker