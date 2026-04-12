#ifndef WISKR_ENTER_KEY_H
#define WISKR_ENTER_KEY_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>
#include <wiskr/events/identifiers.h>

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that sends a key press and release to a target item.
         *
         * The command resolves the target item from the Scene using the provided
         * `ItemPath` and calls the scene event methods to emit a key press followed
         * by a key release. Typical use is to simulate the Enter key or other
         * keyboard input that should be delivered to a particular UI element.
         */
        class WISKRCORE_EXPORT EnterKey : public Command {
        public:

            /**
             * @brief Create an EnterKey command.
             *
             * @param targetPath Path identifying the item that should receive the key events.
             * @param keyCode The integer key code to deliver (platform/application specific).
             * @param mod Key modifiers to apply (e.g. Shift, Control) as defined in `KeyModifier`.
             *
             * The path is copied into the command so it remains valid while queued.
             */
            EnterKey(const ItemPath& targetPath, int keyCode, KeyModifier mod);


            /**
             * @brief Execute the command against the provided Environment.
             *
             * Behavior:
             * - Resolve the item via `env.scene().itemAt(targetPath_)`.
             * - If the item is not found, report an error using `env.state().reportError(...)`.
             * - Otherwise, dispatch `keyPress` and `keyRelease` events to the scene's event system
             *   for the resolved item with the configured `keyCode_` and `mod_`.
             *
             * @param env Execution environment providing access to the Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item for the key events.
            int keyCode_; ///< The key code to send in the key events.
            KeyModifier mod_; ///< The key modifiers to apply in the key events.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_ENTER_KEY_H
