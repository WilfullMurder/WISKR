#ifndef WISKR_DRAG_END_H
#define WISKR_DRAG_END_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

/**
 * @file libs/core/include/wiskr/commands/drag_end.h
 * @brief Command to end a drag operation on a scene item.
 */

namespace whisker {
    namespace cmd {
        /**
         * @file libs/core/include/wiskr/commands/drag_end.h
         * @brief Command to end a drag operation on a scene item.
         *
         * This header declares the DragEnd command used by the WISKR command
         * dispatcher to synthesize the final events required to complete a drag
         * sequence on a target item.
         */


        /**
         * @brief Command that ends a previously started drag on an item.
         *
         * The DragEnd command locates the target item identified by an ItemPath
         * and synthesizes the mouseMove and mouseUp events necessary to finish
         * a drag gesture. If the target item cannot be found at execution time,
         * an error is reported to the shared State object via Environment.
         */
        class WISKRCORE_EXPORT DragEnd : public Command {
        public:

            /**
             * @brief Construct a DragEnd command for the specified item.
             *
             * @param itemPath Path identifying the target item to end dragging.
             *                 The path is copied into the command so it remains
             *                 valid while queued in the Dispatcher.
             */
            DragEnd(const ItemPath& itemPath);

            /**
             * @brief Execute the drag-end action on the provided Environment.
             *
             * Implementation details:
             * - Resolve the item using env.scene().itemAt(itemPath_).
             * - If the item is not found, call env.state().reportError(...) and return.
             * - Otherwise compute a point (typically the center of the item) and
             *   dispatch mouseMove followed by mouseUp events to conclude the drag.
             *
             * @param env Reference to the execution Environment providing access
             *            to the Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath itemPath_; ///< The path to the item to end dragging.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_DRAG_END_H
