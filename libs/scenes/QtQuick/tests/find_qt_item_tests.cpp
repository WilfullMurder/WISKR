#include <gtest/gtest.h>
#include <QQuickWindow>
#include <QQuickItem>

#include <wiskr/find_qt_item.h>
#include <wiskr/data/item_path.h>
#include <wiskr/data/item_path_component.h>
#include <wiskr/qt_item_tools.h>
#include "test_item.h"

using namespace whisker;
using namespace whisker::path;



TEST(FindQtItem, FindsWindowByNameRootSelector) {
    QQuickWindow window;
    window.setObjectName("win_find_test");

    ItemPath path(std::vector<Component>{ Component(NameSelector("win_find_test")) });
    QQuickWindow* found = qt::getQQuickWindowAt(path);

    EXPECT_EQ(found, &window);
}

TEST(FindQtItem, FindsChildByTypeUsingPropertySelector) {
    QQuickWindow window;
    window.setObjectName("win_type_test");

    TestItem child(window.contentItem());
    child.setObjectName("child_type_test");

    // TypeStringForObject will strip 'QQuick' prefix from the class name -> "TestItem"
    ItemPath path(std::vector<Component>{ Component(NameSelector("win_type_test")), Component(PropertySelector("TestItem")) });

    QQuickItem* found = qt::getQQuickItemAt(path);
    EXPECT_EQ(found, &child);
}

TEST(FindQtItem, FindsChildByValueSelectorOnTextProperty) {
    QQuickWindow window;
    window.setObjectName("win_value_test");

    TestItem child(window.contentItem());
    child.setObjectName("child_value_test");
    child.setProperty("text", "hello_value");

    ItemPath path(std::vector<Component>{ Component(NameSelector("win_value_test")), Component(ValueSelector("hello_value")) });

    QQuickItem* found = qt::getQQuickItemAt(path);
    EXPECT_EQ(found, &child);
}

TEST(FindQtItem, FindsChildByPropertyValueSelector) {
    QQuickWindow window;
    window.setObjectName("win_propval_test");

    TestItem child(window.contentItem());
    child.setObjectName("child_propval_test");
    child.setProperty("dataKey", "dataValue");

    ItemPath path(std::vector<Component>{ Component(NameSelector("win_propval_test")), Component(PropertyValueSelector("dataKey", "dataValue")) });

    QQuickItem* found = qt::getQQuickItemAt(path);
    EXPECT_EQ(found, &child);
}

TEST(FindQtItem, TraversesNestedChildrenAndFindsDeeplyNestedByName) {
    QQuickWindow window;
    window.setObjectName("win_traverse_test");

    TestItem parent(window.contentItem());
    parent.setObjectName("parent_item");

    TestItem child(&parent);
    child.setObjectName("nested_child");

    ItemPath path(std::vector<Component>{ Component(NameSelector("win_traverse_test")), Component(NameSelector("parent_item")), Component(NameSelector("nested_child")) });

    QQuickItem* found = qt::getQQuickItemAt(path);
    EXPECT_EQ(found, &child);
}
