#ifndef WISKR_DRAG_BEGIN_H
#define WISKR_DRAG_BEGIN_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>
#include <wiskr/dispatch/environment.h>

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that begins a drag on the item at the given path.
         *
         * When executed, the command locates the target item in the Scene using
         * the provided ItemPath and synthesizes the appropriate mouse events to
         * start a drag (typically mouseDown followed by mouseMove). If the item
         * cannot be found the command should report an error via Environment::state().
         *
         * This class inherits from Command and can be submitted to a Dispatcher.
         */
        class WISKRCORE_EXPORT DragBegin : public Command {
        public:

            /**
             * @brief Construct a DragBegin command.
             *
             * @param itemPath Path identifying the target item to begin dragging.
             *                 The path is copied into the command for later execution.
             */
            DragBegin(const ItemPath& itemPath);

            /**
             * @brief Execute the drag-begin action on the provided Environment.
             *
             * The implemention should:
             * - Resolve the item using env.scene().itemAt(itemPath_).
             * - If the item is not found, report an error on env.state().
             * - Otherwise, compute a suitable point (e.g. center of the item)
             *   and dispatch mouseDown and mouseMove events to begin the drag.
             *
             * @param env Reference to the execution Environment providing access
             *            to the Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath itemPath_; ///< The path to the item to begin dragging. This is stored as a member variable for use during execution.
        };

    } // namespace cmd
} // namespace whisker


#endif //WISKR_DRAG_BEGIN_H
