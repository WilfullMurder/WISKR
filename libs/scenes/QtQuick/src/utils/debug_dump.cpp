#include <wiskr/utils/debug_dump.h>

#include <QMetaObject>
#include <QObject>
#include <QQuickItem>

#include <iostream>

namespace {
    void helper_indent_by(int lvl) {
        for (int i = 0; i < lvl; ++i) {
            std::cout << ".  ";
        }
    }
} // namespace

namespace whisker {
    namespace utils {
        void DumpObject(const QObject *obj) {
            auto meta = obj->metaObject();
            std::cout << "\"" << obj->objectName().toStdString() << "\" [" << meta->className() << "]";
        }

        void DumpObjectTree(const QObject *obj, int lvl) {
            helper_indent_by(lvl);
            DumpObject(obj);
            std::cout << std::endl;
            for (auto child: obj->children()) {
                DumpObjectTree(child, lvl + 1);
            }
        }

        void DumpMetaObject(const QMetaObject *meta) {
            std::cout << "Methods:" << std::endl;
            for (int i = 0; i < meta->methodCount(); ++i){
                auto method = meta->method(i);
                auto signatureBytes = method.methodSignature();
                std::cout << "-- ( " << method.typeName() << " ) " << QString::fromLatin1(signatureBytes).toStdString() << std::endl;
            }
        }

        void DumpQQuickItemTree(const QQuickItem *item, int lvl) {
            helper_indent_by(lvl);

            DumpObject(item);
            std::cout << std::endl;

            for (auto child: item->childItems()) {
                DumpQQuickItemTree(child, lvl + 1);
            }
        }

        void DumpQQuickItemOrQObjectTree(const QObject *obj, int lvl) {
            helper_indent_by(lvl);
            DumpObject(obj);
            std::cout << std::endl;

            if (auto qquickitem = qobject_cast<const QQuickItem *>(obj)) {
                for (auto child: qquickitem->childItems()) {
                    DumpQQuickItemOrQObjectTree(child, lvl + 1);
                }
            } else {
                for (auto child: obj->children()) {
                    DumpQQuickItemOrQObjectTree(child, lvl + 1);
                }
            }
        }
    } // namespace utils
} // namespace whisker