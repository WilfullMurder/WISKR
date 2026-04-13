#ifndef WISKR_SET_PROPERTY_H
#define WISKR_SET_PROPERTY_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

/**
 * @file `libs/core/include/wiskr/commands/set_property.h`
 * @brief Command to set a string property on a scene item.
 *
 * The SetProperty command is posted to the Dispatcher and executed on the
 * Scene thread. It resolves the item identified by an ItemPath and sets the
 * specified string property to the provided value. If the target item cannot
 * be found the command reports an error via the shared State.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that sets a string property on a scene item.
         *
         * Construct an instance with the path to the target item, the name of the
         * property to set and the value to assign. Execution occurs on the scene
         * thread and delegates to the item's setStringProperty method. On failure
         * (item not found) an error is reported through Environment::state().
         */
        class WISKRCORE_EXPORT SetProperty : public Command {
        public:

            /**
             * @brief Create a SetProperty command.
             *
             * @param targetPath Path identifying the item whose property will be set.
             * @param propertyName Name of the string property to modify.
             * @param value New value to assign to the property.
             */
            SetProperty(const ItemPath& targetPath, const std::string& propertyName, const std::string& value);


            /**
             * @brief Execute the command.
             *
             * Behavior:
             * - Resolve the target item via env.scene().itemAt(targetPath_).
             * - If the item is not found: report an error using env.state().reportError(...)
             *   and return without modifying anything.
             * - Otherwise call item->setStringProperty(propertyName_, value_).
             *
             * @param env Execution environment providing access to Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item whose property should be set.
            std::string propertyName_; ///< The name of the string property to set on the target item.
            std::string value_; ///< The new value to assign to the property on the target item.
        };

    } // namespace cmd
} // namespace whisker


#endif //WISKR_SET_PROPERTY_H
