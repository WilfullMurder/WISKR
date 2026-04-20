#include <gtest/gtest.h>
#include <QQuickWindow>
#include <QQuickItem>
#include <wiskr/qt_item.h>
#include "test_item.h"

using namespace whisker;

TEST(QtItem, SizeAndBoundsReflectChildDimensionsAndPosition) {
    QQuickWindow window;
    TestItem child(window.contentItem());
    child.setObjectName("child_item");
    child.setWidth(50);
    child.setHeight(30);
    child.setX(11);
    child.setY(22);

    QtItem qi(&child);

    auto s = qi.size();
    EXPECT_DOUBLE_EQ(s.width, child.width());
    EXPECT_DOUBLE_EQ(s.height, child.height());

    auto pos = qi.position();
    QPointF expected = child.mapToGlobal(QPointF(0, 0));
    EXPECT_DOUBLE_EQ(pos.x, expected.x());
    EXPECT_DOUBLE_EQ(pos.y, expected.y());

    auto bounds = qi.bounds();
    EXPECT_DOUBLE_EQ(bounds.size.width, s.width);
    EXPECT_DOUBLE_EQ(bounds.size.height, s.height);
    EXPECT_DOUBLE_EQ(bounds.topLeft.x, pos.x);
    EXPECT_DOUBLE_EQ(bounds.topLeft.y, pos.y);
}

TEST(QtItem, HandlesWindowConstructionSizeMatchesContentItem) {
    QQuickWindow window;
    auto content = window.contentItem();
    content->setWidth(80);
    content->setHeight(60);

    QtItem qi(&window);

    auto s = qi.size();
    EXPECT_DOUBLE_EQ(s.width, content->width());
    EXPECT_DOUBLE_EQ(s.height, content->height());
}

TEST(QtItem, StringPropertySetAndGetOnWindow) {
    QQuickWindow window;
    QtItem qi(&window);

    qi.setStringProperty("objectName", "main_window");
    EXPECT_EQ(qi.stringProperty("objectName"), std::string("main_window"));
    EXPECT_EQ(window.objectName().toStdString(), "main_window");
}