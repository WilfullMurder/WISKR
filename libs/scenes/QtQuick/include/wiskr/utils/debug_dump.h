#ifndef WISKR_DEBUG_DUMP_H
#define WISKR_DEBUG_DUMP_H

#include <wiskr/wiskr_qtquick_export.h>

class QObject;
struct MetaObject;
class QQuickItem;

namespace wiskr {
    namespace utils {
        WISKRQTQUICK_EXPORT void DumpObject(const QObject *obj);
        WISKRQTQUICK_EXPORT void DumpObjectTree(const QObject *obj, int lvl = 0);
        WISKRQTQUICK_EXPORT void DumpMetaObject(const MetaObject *meta);
        WISKRQTQUICK_EXPORT void DumpQQuickItemTree(const QQuickItem *item, int lvl = 0);
        WISKRQTQUICK_EXPORT void DumpQQuickItemOrQObjectTree(const QObject *obj, int lvl = 0);
    } // namespace utils
} // namespace wiskr

#endif //WISKR_DEBUG_DUMP_H
