#include "mock_events.h"
namespace whisker {

    void MockEvents::mouseDown(Item* item, Point loc, MouseButton /*button*/, KeyModifier /*mod*/) {
        if (onMouseClickEvent) {
            onMouseClickEvent(item, loc, true, false);
        }
    }

    void MockEvents::mouseUp(Item* item, Point loc, MouseButton /*button*/, KeyModifier /*mod*/) {
            if (onMouseClickEvent) {
                onMouseClickEvent(item, loc, false, true);
            }
    }

    void MockEvents::mouseMove(Item* item, Point loc) {
            if (onMouseClickEvent) {
                onMouseClickEvent(item, loc, false, false);
            }
    }

    void MockEvents::stringInput(Item* item, const std::string& text) {
        if (onStringInputEvent) {
            onStringInputEvent(item, text);
        }
    }

    void MockEvents::keyPress(Item* item, int keyCode, KeyModifier mod) {
        // no op for mock
    }

    void MockEvents::keyRelease(Item* item, int keyCode, KeyModifier mod) {
        // no op for mock
    }

    void MockEvents::extMouseDrop(Item* item, Point loc, PasteboardContent& content) {
        if (onExtMouseDropEvent) {
            onExtMouseDropEvent(item, loc, content);
        }
    }

    void MockEvents::quit() {
            // no op for mock
    }

} // namespace whisker
