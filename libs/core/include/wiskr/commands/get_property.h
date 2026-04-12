#ifndef WISKR_GET_PROPERTY_H
#define WISKR_GET_PROPERTY_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

#include <future>

/**
 * @file libs/core/include/wiskr/commands/get_property.h
 * @brief Command to retrieve a property value from a scene item.
 *
 * This header declares the GetProperty command, which is used to obtain the
 * value of a specific property from an item in the scene identified by an ItemPath.
 * The command executes on the main scene thread and fulfills a std::promise<std::string>
 * with the property value if the item and property are found, or an empty string if not.
 *
 * The command can be submitted to a Dispatcher and will safely access the scene's
 * item hierarchy to retrieve property information.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that reads a named string property from a scene item.
         *
         * GetProperty captures the target item path and the property name to read.
         * A std::promise<std::string> is moved into the command and will be
         * satisfied with the property's value when the command executes. Execution
         * happens on the scene thread and uses Environment to access the Scene
         * and State for error reporting.
         */
        class WISKRCORE_EXPORT GetProperty : public Command {
        public:

            /**
             * @brief Construct the GetProperty command.
             *
             * @param targetPath Path identifying the item to query.
             * @param propertyName Name of the string property to retrieve.
             * @param promise Promise that will be fulfilled with the property's value
             *                (or an empty string on failure). The promise is moved
             *                into the command.
             */
            GetProperty(const ItemPath& targetPath, const std::string& propertyName, std::promise<std::string> promise);


            /**
             * @brief Execute the command.
             *
             * Behavior:
             * - Resolve the item via env.scene().itemAt(targetPath_).
             * - If the item is not found: set the promise to an empty string and
             *   report an error on env.state().
             * - Otherwise obtain the property's value using item->stringProperty(propertyName_)
             *   and set the promise with that value.
             *
             * @param env Execution Environment providing access to the Scene and State.
            */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item from which to read the property.
            std::string propertyName_; ///< The name of the property to retrieve from the item.
            std::promise<std::string> promise_; ///< Promise to be fulfilled with the property's value when the command executes.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_GET_PROPERTY_H
