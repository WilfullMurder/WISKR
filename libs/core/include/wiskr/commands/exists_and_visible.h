#ifndef WISKR_EXISTS_AND_VISIBLE_H
#define WISKR_EXISTS_AND_VISIBLE_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

#include <future>

/**
 * @file libs/core/include/wiskr/commands/exists_and_visible.h
 * @brief Command to check if an item exists and is visible in the scene.
 *
 * This header declares the ExistsAndVisible command, which is used to determine
 * whether a specific item identified by an ItemPath exists in the scene and is
 * currently visible. The command executes synchronously and returns the result
 * via a std::promise<bool> that is fulfilled with true if the item exists and
 * is visible, or false otherwise.
 *
 * The command can be submitted to a Dispatcher and will be executed on the main
 * scene thread, allowing it to safely access the scene's item hierarchy and
 * visibility state.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command used to determine existence and visibility of a scene item.
         *
         * The command captures the target ItemPath and a promise which will be
         * satisfied once the command is executed on the scene thread. The promise
         * receives \c false when the item is not present; otherwise it receives
         * the value of item->isVisible().
         */
        class WISKRCORE_EXPORT ExistsAndVisible : public Command {
        public:

            /**
             * @brief Construct the command.
             *
             * @param targetPath The path identifying the target item to check.
             * @param promise A std::promise<bool> that will be fulfilled with the
             *                result when the command executes. The promise is moved
             *                into the command and satisfied on execution.
             */
            explicit ExistsAndVisible(const ItemPath& targetPath, std::promise<bool> promise);

            /**
               * @brief Execute the check against the provided Environment.
               *
               * Behavior:
               * - Resolve the target item via env.scene().itemAt(targetPath_).
               * - If the item is not found, set the promise value to false.
               * - Otherwise set the promise value to the result of item->isVisible().
               *
               * @param env The execution Environment providing access to Scene and State.
               */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item for the existence and visibility check.
            std::promise<bool> promise_; ///< The promise to be fulfilled with the result of the check.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_EXISTS_AND_VISIBLE_H
