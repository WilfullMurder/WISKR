#include <wiskr/utils/qt_event_recorder.h>

#include <QEvent>
#include <QMouseEvent>
#include <iostream>

namespace whisker {
    namespace utils {

        bool QtEventRecorder::eventFilter(QObject* watched, QEvent* event) {
            if(event->type() == QEvent::MouseButtonPress) {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                std::cout << "Button: " << mouseEvent->button() << " Buttons: " << mouseEvent->buttons() << std::endl;
            }
            if(event->type() == QEvent::MouseButtonRelease) {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                std::cout << "Button: " << mouseEvent->button() << " Buttons: " << mouseEvent->buttons() << std::endl;
            }
            return false;
        }
    } // namespace utils
} // namespace whisker