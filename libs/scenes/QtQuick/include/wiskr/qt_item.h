#ifndef WISKR_QT_ITEM_H
#define WISKR_QT_ITEM_H

#include <wiskr/wiskr_qtquick_export.h>
#include <wiskr/scene/item.h>
#include <skitr/core/value.h>

class QObject;
class QQuickItem;
class QQuickWindow;

namespace whisker {

    class QtItem : public Item {
    public:
        QtItem() = delete;
        explicit QtItem(QQuickItem* item);
        explicit QtItem(QQuickWindow* window);

        Size size() const override;
        Point position() const override;
        Rect bounds() const override;
        std::string stringProperty(const std::string& name) const override;
        void setStringProperty(const std::string& name, const std::string& value) override;
        bool invokeMethod(const std::string& method, const std::vector<skitter::core::Value>& args, skitter::core::Value& ret) override;
        bool isVisible() const override;

        QQuickItem* qquickItem();
        const QQuickItem* qquickItem() const;

    private:
        QObject* qobject();
        const QObject* qobject() const;

        std::variant<QQuickItem*, QQuickWindow*> item_; ///< The underlying Qt item, which can be either a QQuickItem or a QQuickWindow.
    };

} // namespace whisker

#endif //WISKR_QT_ITEM_H
