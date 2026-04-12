#ifndef WISKR_DROP_FROM_EXT_H
#define WISKR_DROP_FROM_EXT_H

#include <wiskr/wiskr_core_export.h>

#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>
#include <wiskr/data/pasteboard_content.h>
#include <wiskr/dispatch/environment.h>

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that performs an external drop onto a scene item.
         *
         * DropFromExt carries the target ItemPath and a PasteboardContent payload.
         * When executed on the main scene thread it resolves the item and invokes
         * Scene::events().extMouseDrop() at an appropriate location (typically
         * the center of the item). If the target item cannot be found the
         * command reports an error via Environment::state().
         */
        class WISKRCORE_EXPORT DropFromExt : public Command {
        public:

            /**
             * @brief Construct a DropFromExt command.
             *
             * @param targetPath Path that identifies the target item to receive the drop.
             * @param content PasteboardContent containing the data (e.g. URLs, files)
             *                to be delivered as part of the external drop.
             *
             * Both parameters are copied into the command so it remains self-contained
             * while queued in the Dispatcher.
             */
            DropFromExt(const ItemPath& targetPath, const PasteboardContent& content);

            /**
             * @brief Execute the drop operation.
             *
             * Implementation behavior:
             * - Resolve the item from env.scene().itemAt(targetPath_).
             * - If the item is not found, call env.state().reportError(...) and return.
             * - Otherwise compute a suitable drop point (commonly the center of the item)
             *   and call env.scene().events().extMouseDrop(item.get(), dropPoint, content_).
             *
             * @param env Execution environment providing access to the Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item for the drop. Stored as a member variable for use during execution.
            PasteboardContent content_; ///< The content to be dropped onto the target item. Stored as a member variable for use during execution.
        };

    } // namespace cmd
} // namespace whisker


#endif //WISKR_DROP_FROM_EXT_H
