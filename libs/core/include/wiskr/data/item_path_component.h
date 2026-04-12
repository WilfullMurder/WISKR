#ifndef WISKR_ITEM_PATH_COMPONENT_H
#define WISKR_ITEM_PATH_COMPONENT_H

#include <wiskr/wiskr_core_export.h>

#include <string>
#include <variant>

namespace whisker {
    namespace path {

        /**
         * @file item_path_component.h
         * @brief Types representing the selectable components that make up an item path.
         *
         * This header declares a set of lightweight selector types used to describe
         * parts of an item path and a variant type representing any one selector.
         * It also declares a Component wrapper class that can be constructed from
         * a raw string or an explicit selector.
         *
         * Note: The selector types are value objects intended to be cheap to construct
         * and copy. Their accessors return const references to the stored strings.
         */

        /**
         * @class NameSelector
         * @brief Selects items by their name.
         *
         * Construct with a name string to represent a selector that matches items
         * with the given name.
         */
        class WISKRCORE_EXPORT NameSelector {
        public:
            NameSelector() = default;
            explicit NameSelector(std::string name);

            const std::string& name() const;

        private:
            std::string name_;
        };

        /**
         * @class PropertySelector
         * @brief Selects items by a named property.
         *
         * Represents a selector that will match items having a specific property key.
         */
        class WISKRCORE_EXPORT PropertySelector {
        public:
            PropertySelector() = default;
            explicit PropertySelector(std::string property);
            const std::string& property() const;
        private:
            std::string property_;
        };

        /**
         * @class TypeSelector
         * @brief Selects items by their type identifier.
         *
         * Use this selector to filter items by a type string (for example, class name or tag).
         */
        class WISKRCORE_EXPORT TypeSelector {
        public:
            TypeSelector() = default;
            explicit TypeSelector(std::string type);
            const std::string& type() const;
        private:
            std::string type_;
        };

        /**
         * @class ValueSelector
         * @brief Selects items by a value string.
         *
         * Matches items containing a specific value (semantics depend on item model).
         */
        class WISKRCORE_EXPORT ValueSelector {
        public:
            ValueSelector() = default;
            explicit ValueSelector(std::string value);
            const std::string& value() const;
        private:
            std::string value_;
        };

        /**
         * @class PropertyValueSelector
         * @brief Selects items by matching a property name and a corresponding value.
         *
         * Useful for queries like "property==value".
         */
        class WISKRCORE_EXPORT PropertyValueSelector {
        public:
            PropertyValueSelector() = default;
            PropertyValueSelector(std::string property, std::string value);
            const std::string& property() const;
            const std::string& value() const;
        private:
            std::string property_;
            std::string value_;
        };

        using Selector = std::variant<NameSelector, PropertySelector, TypeSelector, ValueSelector, PropertyValueSelector>;

        /**
         * @class Component
         * @brief A wrapper representing a single component of an item path.
         *
         * The wrapper can be constructed from a raw string (parsing rules not defined here)
         * or directly from a selector instance. It provides accessors to retrieve the
         * underlying selector and a string representation.
         *
         * Note: The header declares the interface; implementations define parsing and
         * conversion behavior.
         */
        class WISKRCORE_EXPORT Component {
            public:
            Component() = default;
            explicit Component(const std::string& raw);
            explicit Component(const Selector& selector);

            std::string string() const;
            const Selector& selector() const;

        private:
            Selector selector_; ///< The selector that defines how to select items at this path component.
        };
    }// namespace path
}// namespace whisker

#endif //WISKR_ITEM_PATH_COMPONENT_H
