#include <wiskr/qt_item.h>

#include <QMetaObject>
#include <QQuickItem>
#include <QQuickWindow>

#include <wiskr/qt_item_tools.h>

namespace whisker {
    QtItem::QtItem(QQuickItem* item): item_(item) {}
    QtItem::QtItem(QQuickWindow* window): item_(window) {}

    Size QtItem::size() const {
        return Size {qquickItem()->width(), qquickItem()->height()};
    }

    Point QtItem::position() const {
        QPointF localPos {0.0, 0.0};
        auto globalPos = qquickItem()->mapToGlobal(localPos);
        return Point{globalPos.x(), globalPos.y()};
    }

    Rect QtItem::bounds() const {
        Rect bounds {0.0, 0.0, 0.0, 0.0};
        bounds.topLeft = position();
        bounds.size = size();
        return bounds;
    }

    std::string QtItem::stringProperty(const std::string &name) const {
        auto value = qobject()->property(name.c_str());
        return value.toString().toStdString();
    }

    void QtItem::setStringProperty(const std::string &name, const std::string &value) {
        qobject()->setProperty(name.c_str(), value.c_str());
    }

    bool QtItem::invokeMethod(const std::string &method, const std::vector<skitter::core::Value> &args, skitter::core::Value &ret) {
        if (args.size() > 10) return false;

        std::vector<QVariant> qtVars;
        for (const auto &arg: args) {
            qtVars.push_back(qt::ValueToQVariant(arg));
        }

        QMetaMethod match;
        bool matched = qt::GetMethodMetaForArgs(*qobject(), method, qtVars, match);
        if (!matched) return false;

        qt::QMLReturnVariant retVar;
        QGenericReturnArgument retArg = qt::GetReturnArgForQMetaType(match.returnType(), retVar);
        bool ok;
        std::vector<QGenericArgument> qtArgs = qt::ConvertAndCCreateQArgumentsForMethod(match, qtVars, ok);
        if (!ok) return false;

        bool success = match.invoke(qobject(), Qt::ConnectionType::DirectConnection, retArg, qtArgs[0], qtArgs[1],
                                    qtArgs[2], qtArgs[3], qtArgs[4], qtArgs[5], qtArgs[6], qtArgs[7], qtArgs[8],
                                    qtArgs[9]);
        if (success) {
            ret = qt::QMLReturnVariantToValue(retVar);
            return true;
        }
        return false;
    }

    bool QtItem::isVisible() const {
        return qquickItem()->isVisible();
    }

    QQuickItem *QtItem::qquickItem() {
        if(std::holds_alternative<QQuickWindow*>(item_)) {
            return std::get<QQuickWindow*>(item_)->contentItem();
        } else {
            return std::get<QQuickItem*>(item_);
        }
    }

    const QQuickItem *QtItem::qquickItem() const {
        if (std::holds_alternative<QQuickWindow*>(item_))
            return std::get<QQuickWindow*>(item_)->contentItem();
        else
            return std::get<QQuickItem*>(item_);
    }

    QObject *QtItem::qobject() {
        return std::visit([](auto i) { return static_cast<QObject*>(i); }, item_);
    }

    const QObject *QtItem::qobject() const {
        return std::visit([](auto i) { return static_cast<const QObject*>(i); }, item_);
    }

} // namespace whisker