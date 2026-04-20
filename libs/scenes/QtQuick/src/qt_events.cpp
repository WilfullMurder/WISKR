#include <wiskr/qt_events.h>

#include <wiskr/qt_item.h>
#include <wiskr/qt_item_tools.h>

#include <wiskr/events/identifiers.h>
#include <wiskr/data/pasteboard_content.h>

#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QUrl>
#include <QList>

namespace whisker{

    namespace {
        QQuickWindow* getWindowAndPositionFor(Item* item, const Point& relativePosition, QPointF& windowPosition) {
            auto qitem = dynamic_cast<QtItem*>(item);
            if(!qitem) return nullptr;

            auto window = qitem->qquickItem()->window();

            QPointF qLocalPoint(relativePosition.x, relativePosition.y);
            windowPosition = qitem->qquickItem()->mapToScene(qLocalPoint);
            return window;
        }

        Qt::MouseButton getQtMouseButtonValue(MouseButton button) {
            unsigned qtButton = Qt::MouseButton::NoButton;

            if (button & MouseButtons::Left)   qtButton |= Qt::LeftButton;
            if (button & MouseButtons::Right)  qtButton |= Qt::RightButton;
            if (button & MouseButtons::Middle) qtButton |= Qt::MiddleButton;

            return static_cast<Qt::MouseButton>(qtButton);
        }

        Qt::KeyboardModifiers getQtKeyModifierValue(KeyModifier mod) {
            Qt::KeyboardModifiers qtMod = Qt::NoModifier;

            if (mod & KeyModifiers::Shift)   qtMod |= Qt::ShiftModifier;
            if (mod & KeyModifiers::Control) qtMod |= Qt::ControlModifier;
            if (mod & KeyModifiers::Alt)     qtMod |= Qt::AltModifier;
            if (mod & KeyModifiers::Meta)    qtMod |= Qt::MetaModifier;

            return qtMod;
        }

        void sendQtKeyEvent(Item* item, bool pressed, int keyCode, KeyModifier mod) {
            auto qitem = dynamic_cast<QtItem *>(item);
            if (!qitem) return;

            auto window = qitem->qquickItem()->window();
            auto qtMod = getQtKeyModifierValue(mod);
            auto keyEvent = new QKeyEvent(pressed ? QEvent::KeyPress : QEvent::KeyRelease, keyCode, qtMod);
            QGuiApplication::postEvent(window, keyEvent);
        }
    } // namespace

    void QtEvents::mouseDown(Item *item, Point loc, MouseButton button, KeyModifier mod) {
        QPointF windowPos;
        auto window = getWindowAndPositionFor(item, loc, windowPos);
        if(!window) return;

        pressedButtons_ |= button;
        // cause expects a single Qt::MouseButton -> cast the flags result
        Qt::MouseButton cause = static_cast<Qt::MouseButton>(getQtMouseButtonValue(button));
        Qt::MouseButtons activeButtons = getQtMouseButtonValue(pressedButtons_);

        auto qtMod = getQtKeyModifierValue(mod);
        QMouseEvent* event = new QMouseEvent(QEvent::MouseButtonPress, windowPos, cause, activeButtons, qtMod);
        QGuiApplication::postEvent(window, event);
    }

    void QtEvents::mouseUp(Item *item, Point loc, MouseButton button, KeyModifier mod) {
        QPointF windowPos;
        auto window = getWindowAndPositionFor(item, loc, windowPos);
        if(!window) return;

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        // Qt6 expects the mouse to be down during the event
        Qt::MouseButton cause = static_cast<Qt::MouseButton>(getQtMouseButtonValue(button));
        Qt::MouseButtons activeButtons = getQtMouseButtonValue(pressedButtons_);
        pressedButtons_ ^= button; // toggle off the released button
#else
        // Qt5 expects the mouse to be up during the event
        pressedButtons_ ^= button; // toggle off the released button
        Qt::MouseButton cause = static_cast<Qt::MouseButton>(getQtMouseButtonValue(button));
        Qt::MouseButtons activeButtons = getQtMouseButtonValue(pressedButtons_);
#endif
        auto qtMod = getQtKeyModifierValue(mod);
        QMouseEvent* event = new QMouseEvent(QEvent::MouseButtonRelease, windowPos, cause, activeButtons, qtMod);
        QGuiApplication::postEvent(window, event);
    }

    void QtEvents::mouseMove(Item *item, Point loc) {
        QPointF windowPos;
        auto window = getWindowAndPositionFor(item, loc, windowPos);
        if(!window) return;

        Qt::MouseButtons activeButtons = getQtMouseButtonValue(pressedButtons_);

        // Wiggle the cursor slightly to trigger drag enter
        windowPos.rx() -= 1;
        QMouseEvent* moveEvent = new QMouseEvent(QEvent::MouseMove, windowPos, Qt::NoButton, activeButtons, Qt::NoModifier);
        QGuiApplication::postEvent(window, moveEvent);

        windowPos.rx() += 1;
        moveEvent = new QMouseEvent(QEvent::MouseMove, windowPos, Qt::NoButton, activeButtons, Qt::NoModifier);
        QGuiApplication::postEvent(window, moveEvent);
    }

    void QtEvents::keyPress(Item* item, int keyCode, KeyModifier mod) {
        sendQtKeyEvent(item, true, keyCode, mod);
    }

    void QtEvents::keyRelease(Item* item, int keyCode, KeyModifier mod) {
        sendQtKeyEvent(item, false, keyCode, mod);
    }

    void QtEvents::stringInput(Item* item, const std::string& text) {
        auto qitem = dynamic_cast<QtItem*>(item);
        if(!qitem) return;

        auto window = qitem->qquickItem()->window();
        QKeyEvent* keyEvent = new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier, QString::fromStdString(text));
        QGuiApplication::postEvent(window, keyEvent);
    }

    void QtEvents::extMouseDrop(Item *item, Point loc, PasteboardContent &content) {
        auto qitem = dynamic_cast<QtItem *>(item);
        if (!qitem) return;

        auto window = qitem->qquickItem()->window();

        QPointF localPos(loc.x, loc.y);
        QPointF windowPos = qitem->qquickItem()->mapToScene(localPos);
        QPoint windowPosInt(windowPos.rx(), windowPos.ry());

        auto mimeData = new QMimeData();
        QList<QUrl> urls;
        for (const auto& url: content.urls()) {
            QUrl qurl(QString::fromStdString(url));
            urls.append(qurl);
        }
        mimeData->setUrls(urls);

        auto enter = new QDragEnterEvent(windowPosInt, Qt::CopyAction, mimeData, Qt::LeftButton, Qt::NoModifier);
        QGuiApplication::postEvent(window, enter);

        auto move = new QDragMoveEvent(windowPosInt, Qt::CopyAction, mimeData, Qt::LeftButton, Qt::NoModifier);
        QGuiApplication::postEvent(window, move);

        auto drop = new QDropEvent(windowPosInt, Qt::CopyAction, mimeData, Qt::LeftButton, Qt::NoModifier);
        QGuiApplication::postEvent(window, drop);
    }

    void QtEvents::quit() {
        QGuiApplication::quit();
    }

} // namespace whisker