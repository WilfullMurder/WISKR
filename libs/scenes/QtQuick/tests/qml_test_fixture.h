#ifndef WISKR_QML_TEST_FIXTURE_H
#define WISKR_QML_TEST_FIXTURE_H
#include <wiskr/data/item_path.h>
#include <wiskr/data/item_path_component.h>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>

struct QmlTestFixture {
    QQmlEngine engine;

    QQuickItem* createItem(const char* qml)
    {
        QQmlComponent component(&engine);
        component.setData(qml, QUrl());

        QObject* obj = component.create();
        if (!obj)
            throw std::runtime_error(component.errorString().toStdString());

        return qobject_cast<QQuickItem*>(obj);
    }
};

namespace test {
    class PathBuilder {
    public:
        PathBuilder& name(const std::string& name) {
            components_.emplace_back(
                    whisker::path::NameSelector{name}
            );
            return *this;
        }

        PathBuilder& property(const std::string& type) {
            components_.emplace_back(
                    whisker::path::PropertySelector{type}
            );
            return *this;
        }

        PathBuilder& value(const std::string& value) {
            components_.emplace_back(
                    whisker::path::ValueSelector{value}
            );
            return *this;
        }

        PathBuilder& propertyValue(const std::string& prop, const std::string& val) {
            components_.emplace_back(
                    whisker::path::PropertyValueSelector{prop, val}
            );
            return *this;
        }

        whisker::ItemPath build() const {
            return whisker::ItemPath{components_};
        }

    private:
        std::vector<whisker::path::Component> components_;
    };

    inline PathBuilder Path() {
        return {};
    }
}

#endif //WISKR_QML_TEST_FIXTURE_H
