#ifndef WISKR_ITEM_H
#define WISKR_ITEM_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/data/geometry.h>
#include <skitr/core/value.h>
#include <string>
#include <vector>

/**
 * @file libs/core/include/wiskr/scene/item.h
 * @brief Declarations for scene Item types used by the WISKR core library.
 *
 * This header provides the public interface for scene items that participate
 * in event handling, property access, and method invocation used by the test
 * and automation layers. It documents the purpose of each include and the
 * role of the `whisker` namespace elements contained in the header.
 */


namespace whisker {

    /**
     * @brief Abstract interface representing an interactive UI item within a Scene.
     *
     * Item is the core abstraction used by WISKR to interact with UI elements in a
     * platform-agnostic way (e.g., QtQuick, QtWidgets, or other backends).
     *
     * Implementations are responsible for bridging between the underlying UI framework
     * and WISKR's command system.
     *
     * Items expose:
     * - Geometry (size, position, bounds)
     * - Property access (string-based for simplicity and portability)
     * - Method invocation (via SKITR Value for RPC-style interaction)
     * - Visibility state
     *
     * This interface is intentionally minimal and runtime-driven to support
     * remote automation and testing scenarios.
     */
    class WISKRCORE_EXPORT Item {
    public:
        virtual ~Item() = default;

        /**
         * @brief Returns the size of the item.
         *
         * @return Size in local coordinates. May be empty (0,0) if the item has no layout.
         */
        virtual Size size() const = 0;

        /**
         * @brief Returns the position of the item.
         *
         * @return Position in parent or scene coordinates (implementation-defined).
         */
        virtual Point position() const = 0;

        /**
         * @brief Returns the bounding rectangle of the item.
         *
         * @return Bounding box in scene or screen coordinates (implementation-defined).
         */
        virtual Rect bounds() const = 0;

        /**
         * @brief Retrieves a string property by name.
         *
         * This is a simplified property interface intended for common UI properties
         * (e.g., "text", "value", "title").
         *
         * @param name Name of the property.
         * @return Property value as string.
         *
         * @note Behavior is implementation-defined if the property does not exist.
         *       Implementations may return an empty string or throw/log an error.
         */
        virtual std::string stringProperty(const std::string& name) const = 0;

        /**
         * @brief Sets a string property by name.
         *
         * @param name Name of the property.
         * @param value New value to assign.
         *
         * @note Implementations should handle invalid properties gracefully.
         */
        virtual void setStringProperty(const std::string& name, const std::string& value) = 0;

        /**
         * @brief Invokes a method on the underlying UI object.
         *
         * This provides a generic mechanism for interacting with framework-specific
         * functionality (e.g., QML methods).
         *
         * @param method Name of the method to invoke.
         * @param args Arguments to pass to the method (SKITR Value types).
         * @param[out] ret Return value of the method call.
         *
         * @return true if the method was successfully invoked, false otherwise.
         *
         * @note This is the primary extension point for advanced interactions.
         */
        virtual bool invokeMethod(const std::string& method, const std::vector<skitter::core::Value>& args, skitter::core::Value& ret) = 0;

        /**
         * @brief Indicates whether the item is currently visible.
         *
         * @return true if the item is visible and can be interacted with.
         */
        virtual bool isVisible() const = 0;
    };
} // namespace whisker


#endif //WISKR_ITEM_H
