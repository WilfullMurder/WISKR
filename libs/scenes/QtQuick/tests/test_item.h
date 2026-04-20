#ifndef WISKR_TEST_ITEM_H
#define WISKR_TEST_ITEM_H
#include <QQuickItem>

class TestItem : public QQuickItem {
    Q_OBJECT
public:
    TestItem(QQuickItem* parent = nullptr);
};

class InvokableItem : public QQuickItem {
Q_OBJECT
public:
    InvokableItem(QQuickItem* parent = nullptr) { if (parent) setParentItem(parent); }

    Q_INVOKABLE int sum(int a, int b) { return a + b; }
    Q_INVOKABLE double half(double v) { return v / 2.0; }
    Q_INVOKABLE QString greet(const QString& who) { return QString("hello ") + who; }
    Q_INVOKABLE QVariant echoVariant(const QVariant& v) { return v; }
};

#endif //WISKR_TEST_ITEM_H
