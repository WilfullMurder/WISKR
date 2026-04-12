#ifndef WISKR_CLICK_ON_ITEM_H
#define WISKR_CLICK_ON_ITEM_H

#include <wiskr/wiskr_core_export.h>

#include <wiskr/commands/command.h>
#include <wiskr/data/item_position.h>
#include <wiskr/scene/events.h>
#include <wiskr/events/identifiers.h>

namespace whisker {
    namespace cmd {

        /**
         * @brief Command to simulate a mouse click on an item at a specific position.
         *
         * This command encapsulates the information required to perform a click:
         * the target ItemPosition, which mouse button to use, and any key modifiers
         * (e.g., Ctrl/Shift). It implements the Command interface and performs the
         * click when executed within an Environment.
         */
        class WISKRCORE_EXPORT ClickOnItem : public Command {
        public:
            /**
            * @brief Construct a ClickOnItem command.
            *
            * @param position The target position on the item (contains item id and coordinates).
            * @param button The mouse button to use for the click. Defaults to MouseButton::Left.
            * @param keyModifier Optional key modifier state (e.g., Shift, Ctrl). Defaults to no modifiers.
            */
            ClickOnItem(const ItemPosition& position, MouseButton button = MouseButtons::Left, KeyModifier keyModifier = KeyModifiers::None);

            void execute(Environment& env) override;

        private:
            ItemPosition position_; ///< The target position on the item to click.
            MouseButton button_; ///< The mouse button to use for the click.
            KeyModifier keyModifier_; ///< Optional key modifier state for the click.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_CLICK_ON_ITEM_H
