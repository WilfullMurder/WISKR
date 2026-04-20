#include <wiskr/find_qt_item.h>

#include <wiskr/qt_item_tools.h>
#include <wiskr/data/item_path_component.h>

#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>

namespace {
    template <typename SelectorType>
    QObject* MatchesSpecificSelector(QObject* item, const SelectorType& specificSelector) {
        // Default implementation for unknown selector types
        return nullptr;
    }

    template <>
    QObject* MatchesSpecificSelector(QObject* item, const whisker::path::NameSelector& specificSelector) {
        if(!item) return nullptr;

        const auto &itemName = specificSelector.name();
        if (whisker::qt::GetObjectName(item) != QString::fromStdString(itemName)) {
            return nullptr;
        }
        return item;
    }

    template <>
    QObject* MatchesSpecificSelector(QObject* item, const whisker::path::PropertySelector& specificSelector) {
        if(!item) return nullptr;

        const auto& property = specificSelector.property();
        if(whisker::qt::TypeStringForObject(item) != QString::fromStdString(property)) {
            return nullptr;
        }
        return item;
    }

    template <>
    QObject* MatchesSpecificSelector(QObject* item, const whisker::path::ValueSelector& specificSelector) {
        if(!item) return nullptr;

        QVariant textProperty = item->property("text");
        if(!textProperty.isValid() || !textProperty.canConvert<QString>()) {
            return nullptr;
        }

        QString textValue = textProperty.toString();
        if(textValue != QString::fromStdString(specificSelector.value())) {
            return nullptr;
        }
        return item;
    }


    template <>
    QObject* MatchesSpecificSelector(QObject* item, const whisker::path::PropertyValueSelector& specificSelector) {
        if(!item) return nullptr;

        const auto& property = specificSelector.property();
        const auto& expectedValue = specificSelector.value();

        QVariant propertyValue = item->property(property.c_str());
        if(!propertyValue.isValid() || !propertyValue.canConvert<QString>()) {
            return nullptr;
        }

        QString actualValue = propertyValue.toString();
        if(actualValue != QString::fromStdString(expectedValue)) {
            return nullptr;
        }
        return item;
    }

    QObject* MatchesSelector(QObject* item, const whisker::path::Selector& selector) {
        return std::visit([item](const auto& specificSelector) -> QObject* {
            return MatchesSpecificSelector(item, specificSelector);
        }, selector);
    }

    QQuickItem* FindMatchingItem(const std::vector<whisker::path::Component>& components, QObject* currentNode, size_t matchedCount) {
        if(!currentNode) return nullptr;
        if(matchedCount >= components.size()) return nullptr;

        const auto& component = components[matchedCount];
        const auto& selector = component.selector();

        QObject* matchedNode = MatchesSelector(currentNode, selector);
        if(matchedNode) {
            matchedCount++;

            if(matchedCount == components.size()) {
                return qobject_cast<QQuickItem*>(matchedNode);
            }

            const auto& nextComponent = components[matchedCount];
            if(matchedNode != currentNode || std::holds_alternative<whisker::path::PropertySelector>(nextComponent.selector())) {
                return FindMatchingItem(components, matchedNode, matchedCount);
            }
        }

        QQuickItem* result = nullptr;

        whisker::qt::ForEachChild(currentNode, [&](QObject* child) -> bool {
            if((result = FindMatchingItem(components, child, matchedCount))) return false; // stop iteration if found
            return true; // continue iteration
        });

        return result;
    }

    QQuickWindow* GetQQuickWindowWithName(const std::string& name) {
        QString qName = QString::fromStdString(name);
        QQuickWindow* result = nullptr;

        auto windows = QGuiApplication::topLevelWindows();
        for (const auto& window : windows) {
            QQuickWindow* quickWindow = qobject_cast<QQuickWindow*>(window);
            if (quickWindow && whisker::qt::GetObjectName(quickWindow) == qName) {
                result = quickWindow;
                break;
            }
        }
        return result;
    }
} // namespace

namespace whisker {
    namespace qt {
        QQuickWindow* getQQuickWindowAt(const ItemPath& path) {
            if(path.length() == 0) return nullptr;

            const auto& windowComponent = path.root();
            const auto& windowSelector = windowComponent.selector();

            if(!std::holds_alternative<whisker::path::NameSelector>(windowSelector)) return nullptr;

            const auto& windowName = std::get<whisker::path::NameSelector>(windowSelector).name();
            QQuickWindow* window = GetQQuickWindowWithName(windowName);
            if(!window) return nullptr;

            return window;
        }

        QQuickItem* getQQuickItemAt(const ItemPath& path) {
            if (path.length() == 0) return nullptr;

            QQuickWindow *window = getQQuickWindowAt(path);
            if (!window) return nullptr;

            if (path.length() == 1) return window->contentItem();

            auto components = path.components();
            auto result = FindMatchingItem(components, window->contentItem(), 1);
            if (!result)
                result = FindMatchingItem(components, window,
                                          1); // try searching from window level if not found in contentItem

            return result;
        }
    } // namespace qt
} // namespace whisker