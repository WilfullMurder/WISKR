#ifndef WISKR_GET_BOUNDING_BOX_H
#define WISKR_GET_BOUNDING_BOX_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/geometry.h>
#include <wiskr/data/item_path.h>

#include <future>

/**
 * @file libs/core/include/wiskr/commands/get_bounding_box.h
 * @brief Command to retrieve the bounding box of a scene item.
 *
 * This header declares the GetBoundingBox command, which is used to obtain the
 * bounding rectangle of a specific item in the scene identified by an ItemPath.
 * The command executes on the main scene thread and fulfills a std::promise<Rect>
 * with the bounding box if the item is found, or an empty Rect if the item does
 * not exist.
 *
 * The command can be submitted to a Dispatcher and will safely access the scene's
 * item hierarchy to retrieve geometry information.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that retrieves the bounding box (Rect) of a scene item.
         *
         * The GetBoundingBox command is enqueued on the Dispatcher and executed
         * on the Scene thread. On execution it resolves the item identified by
         * `targetPath_` and sets the provided promise with the item's bounds.
         * If the item cannot be found, an error is reported via the Environment
         * and the promise is satisfied with a zero Rect.
         */
        class WISKRCORE_EXPORT GetBoundingBox : public Command {
        public:

            /**
             * @brief Construct a GetBoundingBox command.
             *
             * @param targetPath Path identifying the target item whose bounds are requested.
             * @param promise A std::promise\<Rect\> that will be fulfilled with the
             *                bounding rectangle when the command runs. The promise
             *                is moved into the command.
             */
            explicit GetBoundingBox(const ItemPath& targetPath, std::promise<Rect> promise);

            /**
             * @brief Execute the command using the given Environment.
             *
             * Behavior:
             * - Resolve the target item via env.scene().itemAt(targetPath_).
             * - If the item is not found, report an error on env.state() and set
             *   the promise to a Rect with zero size.
             * - Otherwise set the promise to item->bounds().
             *
             * @param env Execution Environment providing access to Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item for which to retrieve the bounding box.
            std::promise<Rect> promise_; ///< The promise to be fulfilled with the bounding box result.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_GET_BOUNDING_BOX_H
