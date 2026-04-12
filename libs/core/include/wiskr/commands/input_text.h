#ifndef WISKR_INPUT_TEXT_H
#define WISKR_INPUT_TEXT_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

/**
 * @file libs/core/include/wiskr/commands/input_text.h
 * @brief Command to input text into a scene item.
 * This header declares the `InputText` command which is enqueued on the
 * Dispatcher and executed on the main scene thread. The command resolves an
 * item identified by an `ItemPath` and emits a string input event via the
 * Scene's event system. If the target item cannot be found at execution time
 * an error is reported to the shared `State`.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that delivers a string input event to a target item.
         *
         * The command stores the target item's path and the text to input. When
         * executed on the scene thread it resolves the item and calls
         * scene().events().stringInput(...) to deliver the text. If the item is
         * not found an error is reported via Environment::state().
         */
        class WISKRCORE_EXPORT InputText : public Command {
        public:

            /**
             * @brief Construct an InputText command.
             *
             * @param targetPath Path identifying the item that should receive the text.
             * @param text The text to be delivered as input to the target item.
             *
             * The `targetPath` and `text` are copied into the command so they
             * remain valid while the command is queued.
             */
            InputText(const ItemPath& targetPath, const std::string& text);


            /**
             * @brief Execute the command using the provided Environment.
             *
             * Behavior:
             * - Resolve the item via `env.scene().itemAt(targetPath_)`.
             * - If the item is not found, report an error using `env.state().reportError(...)`.
             * - Otherwise dispatch a string input event to the scene's event system
             *   for the resolved item with the configured `text_`.
             *
             * @param env Execution environment providing access to the Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item for the text input.
            std::string text_; ///< The text to be delivered as input to the target item.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_INPUT_TEXT_H
