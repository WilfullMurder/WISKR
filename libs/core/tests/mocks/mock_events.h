#ifndef WISKR_MOCK_EVENTS_H
#define WISKR_MOCK_EVENTS_H

#include <wiskr/scene/events.h>
#include <wiskr/data/geometry.h>
#include <wiskr/data/pasteboard_content.h>
#include "mock_item.h"
#include <string>

#include <functional>

namespace whisker {

    class MockEvents: public Events {
    public:

        void mouseDown(Item* item, Point loc, MouseButton button, KeyModifier mod) override;
        void mouseUp(Item* item, Point loc, MouseButton button, KeyModifier mod) override;
        void mouseMove(Item* item, Point loc) override;
        void stringInput(Item* item, const std::string& text) override;
        void keyPress(Item* item, int keyCode, KeyModifier mod) override;
        void keyRelease(Item* item, int keyCode, KeyModifier mod) override;
        void extMouseDrop(Item* item, Point loc, PasteboardContent& content) override;
        void quit() override;

        std::function<void(Item*, Point, MouseButton, KeyModifier)> onMouseClickEvent;
        std::function<void(Item*, const std::string&)> onStringInputEvent;
        std::function<void(Item*, Point, PasteboardContent&)> onExtMouseDropEvent;
    };
} // namespace whisker

#endif //WISKR_MOCK_EVENTS_H
