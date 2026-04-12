#ifndef WISKR_EVENTS_H
#define WISKR_EVENTS_H

#include <wiskr/wiskr_core_export.h>

#include <wiskr/data/geometry.h>
#include <wiskr/data/pasteboard_content.h>
#include <wiskr/events/identifiers.h>
#include <wiskr/scene/item.h>

namespace whisker {
    /**
     * The Events class defines an interface for handling various user input events such as mouse clicks, keyboard input, and drag-and-drop actions
     * Implementations of this interface can be used to respond to user interactions with items in the scene, allowing for interactive behavior in the application.
     */
    class WISKRCORE_EXPORT Events {
    public:
        virtual ~Events() = default;

        /**
         * Handles a mouse down event on an item at a specific location with the given mouse button and key modifiers.
         * @param item The item that received the mouse down event.
         * @param loc The location of the mouse event in scene coordinates.
         * @param button The mouse button that was pressed (e.g., left, right, middle).
         * @param mod The key modifiers that were active during the mouse event (e.g., shift, control, alt).
         */
        virtual void mouseDown(Item* item, Point loc, MouseButton button, KeyModifier mod) = 0;

        /**
         * Handles a mouse up event on an item at a specific location with the given mouse button and key modifiers.
         * @param item The item that received the mouse up event.
         * @param loc The location of the mouse event in scene coordinates.
         * @param button The mouse button that was released (e.g., left, right, middle).
         * @param mod The key modifiers that were active during the mouse event (e.g., shift, control, alt).
         */
        virtual void mouseUp(Item* item, Point loc, MouseButton button, KeyModifier mod) = 0;

        /**
         * Handles a mouse move event on an item at a specific location.
         * @param item The item that received the mouse move event.
         * @param loc The location of the mouse event in scene coordinates.
         */
        virtual void mouseMove(Item* item, Point loc) = 0;

        /**
         * Handles a string input event on an item, typically used for text input.
         * @param item The item that received the string input event.
         * @param text The string of text that was input by the user.
         */
        virtual void stringInput(Item* item, const std::string& text) = 0;

        /**
         * Handles a key press event on an item with the given key code and key modifiers.
         * @param item The item that received the key press event.
         * @param keyCode The code of the key that was pressed (e.g., character code or special key code).
         * @param mod The key modifiers that were active during the key event (e.g., shift, control, alt).
         */
        virtual void keyPress(Item* item, int keyCode, KeyModifier mod) = 0;

        /**
         * Handles a key release event on an item with the given key code and key modifiers.
         * @param item The item that received the key press event.
         * @param keyCode The code of the key that was pressed (e.g., character code or special key code).
         * @param mod The key modifiers that were active during the key event (e.g., shift, control, alt).
         */
        virtual void keyRelease(Item* item, int keyCode, KeyModifier mod) = 0;

        /**
         * Handles an external mouse drop event on an item at a specific location with the given content from the pasteboard.
         * @param item The item that received the external mouse drop event.
         * @param loc The location of the mouse event in scene coordinates.
         * @param content The content from the pasteboard that was dropped onto the item.
         */
        virtual void extMouseDrop(Item* item, Point loc, PasteboardContent& content) = 0;

        virtual void quit() = 0;
    };
}// namespace whisker



#endif //WISKR_EVENTS_H
