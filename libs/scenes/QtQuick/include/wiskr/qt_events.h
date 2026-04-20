#ifndef WISKR_QT_EVENTS_H
#define WISKR_QT_EVENTS_H

#include <wiskr/wiskr_qtquick_export.h>
#include <wiskr/scene/events.h>
#include <wiskr/events/identifiers.h>

/**
 * @file libs/scenes/QtQuick/include/wiskr/qt_events.h
 * @brief Declarations and documentation for Qt event helper used by Wiskr's QtQuick scene.
 *
 * This header declares the interface for translating Wiskr `Events` operations into Qt GUI events.
 * The implementation lives in `libs/scenes/QtQuick/src/qt_events.cpp`.
 *
 * The QtEvents helper provides functions to synthesize and post Qt events (mouse, keyboard,
 * drag/drop, and text input) against QQuickItem/QQuickWindow targets represented by `QtItem`.
 *
 * Notes:
 *  - This header is part of the WISKR QtQuick module and depends on Qt Core/Gui/Quick.
 *  - The test-suite uses these helpers to simulate user interactions in unit tests.
 */


namespace whisker {


   class WISKRQTQUICK_EXPORT QtEvents: public Events{

   public:
       void mouseDown(Item *item, Point loc, MouseButton button, KeyModifier mod) override;

       void mouseUp(Item *item, Point loc, MouseButton button, KeyModifier mod) override;

       void mouseMove(Item *item, Point loc) override;

       void stringInput(Item *item, const std::string &text) override;

       void keyPress(Item *item, int keyCode, KeyModifier mod) override;

       void keyRelease(Item *item, int keyCode, KeyModifier mod) override;

       void extMouseDrop(Item *item, Point loc, PasteboardContent &content) override;

       void quit() override;

   private:
       MouseButton pressedButtons_ = MouseButtons::None; ///< Track currently pressed mouse buttons
   };
} // namespace whisker

#endif //WISKR_QT_EVENTS_H
