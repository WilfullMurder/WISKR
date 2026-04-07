#ifndef WISKR_WIDGET_REGISTRY_H
#define WISKR_WIDGET_REGISTRY_H

#include <QWidget>
#include <QPointer>
#include <QRect>
#include <QMutex>
#include <QString>
#include <QVector>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <cstdint>

#include <wiskr/wiskr_core_export.h>

namespace whisker {
    namespace core {

        struct WidgetInfo {
            std::uint64_t id;
            QPointer<QWidget> widget;
            QString objectName;
            QString className;
            QRect geometry;
            bool visible;
        };


    } // namespace core
} // namespace whisker

#endif //WISKR_WIDGET_REGISTRY_H
