#ifndef WISKR_QT_EVENT_RECORDER_H
#define WISKR_QT_EVENT_RECORDER_H

#include <wiskr/wiskr_qtquick_export.h>

#include <QObject>

namespace whisker {
    namespace utils {

        class WISKRQTQUICK_EXPORT QtEventRecorder : public QObject {
        public:
            bool eventFilter(QObject *watched, QEvent *event);
        };
    } // namespace utils
} // namespace whisker

#endif //WISKR_QT_EVENT_RECORDER_H
