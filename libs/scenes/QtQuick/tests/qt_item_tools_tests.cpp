#include <gtest/gtest.h>
#include <QQuickWindow>
#include <QQuickItem>
#include <QString>
#include <QSignalSpy>

#include <wiskr/qt_item.h>
#include <wiskr/qt_item_tools.h>
#include <skitr/core/value.h>
#include "qml_test_fixture.h"
#include "test_item.h"

using namespace whisker;



TEST(QtItemTools, ValueToQVariantAndBack_PrimitiveAndString) {
using skitter::core::Value;

Value vi(int64_t(42));
QVariant qvi = qt::ValueToQVariant(vi);
auto vi_back = qt::QVariantToValue(qvi);
EXPECT_TRUE(vi_back.isInt());
EXPECT_EQ(vi_back.asInt(), 42);

Value vd(3.14);
QVariant qvd = qt::ValueToQVariant(vd);
auto vd_back = qt::QVariantToValue(qvd);
EXPECT_TRUE(vd_back.isDouble());
EXPECT_DOUBLE_EQ(vd_back.asDouble(), 3.14);

Value vs(std::string("xyz"));
QVariant qvs = qt::ValueToQVariant(vs);
auto vs_back = qt::QVariantToValue(qvs);
EXPECT_TRUE(vs_back.isString());
EXPECT_EQ(vs_back.asString(), "xyz");
}

TEST(QtItemTools, ValueToQVariantAndBack_ContainerTypes) {
using skitter::core::Value;

Value::array_t arr = { Value(int64_t(1)), Value(std::string("a")) };
Value varr(arr);
QVariant qarr = qt::ValueToQVariant(varr);
auto varr_back = qt::QVariantToValue(qarr);
EXPECT_TRUE(varr_back.isArray());
EXPECT_EQ(varr_back.asArray().size(), 2);
EXPECT_EQ(varr_back.asArray()[0].asInt(), 1);
EXPECT_EQ(varr_back.asArray()[1].asString(), "a");

Value::object_t obj;
obj["k"] = Value(std::string("v"));
Value vobj(obj);
QVariant qobj = qt::ValueToQVariant(vobj);
auto vobj_back = qt::QVariantToValue(qobj);
EXPECT_TRUE(vobj_back.isObject());
EXPECT_EQ(vobj_back.asObject().at("k").asString(), "v");
}

TEST(QtItem, StringPropertySetAndGetAndVisibility) {
QQuickWindow window;
InvokableItem item(window.contentItem());

QtItem qi(&item);
qi.setStringProperty("customProp", "abc123");
EXPECT_EQ(qi.stringProperty("customProp"), std::string("abc123"));

// objectName through property API
qi.setStringProperty("objectName", "myname");
EXPECT_EQ(qi.stringProperty("objectName"), std::string("myname"));

// visibility
item.setVisible(false);
EXPECT_FALSE(qi.isVisible());
item.setVisible(true);
EXPECT_TRUE(qi.isVisible());
}

TEST(QtItemInvokeMethod, InvokesMethodWithArgsAndReturnsConvertedValue) {
QQuickWindow window;
InvokableItem item(window.contentItem());

QtItem qi(&item);

std::vector<skitter::core::Value> args;
args.emplace_back(int64_t(2));
args.emplace_back(int64_t(3));

skitter::core::Value ret;
bool ok = qi.invokeMethod("sum", args, ret);
EXPECT_TRUE(ok);
EXPECT_TRUE(ret.isInt());
EXPECT_EQ(ret.asInt(), 5);

// double argument and return
args.clear();
args.emplace_back(10.0);
skitter::core::Value ret2;
ok = qi.invokeMethod("half", args, ret2);
EXPECT_TRUE(ok);
EXPECT_TRUE(ret2.isDouble());
EXPECT_DOUBLE_EQ(ret2.asDouble(), 5.0);

// string return
args.clear();
args.emplace_back(std::string("you"));
skitter::core::Value ret3;
ok = qi.invokeMethod("greet", args, ret3);
EXPECT_TRUE(ok);
EXPECT_TRUE(ret3.isString());
EXPECT_EQ(ret3.asString(), "hello you");
}

TEST(QtItemInvokeMethod, FailsWhenTooManyArguments) {
QQuickWindow window;
InvokableItem item(window.contentItem());
QtItem qi(&item);

std::vector<skitter::core::Value> args(11, skitter::core::Value());
skitter::core::Value ret;
bool ok = qi.invokeMethod("sum", args, ret);
EXPECT_FALSE(ok);
}


TEST(QtItemTools, ValueToQVariantAndBack_BinaryAndDateTime) {
using skitter::core::Value;

Value::binary_t bin = { 0x01, 0x02, 0xFF };
Value vb(bin);
QVariant qbin = qt::ValueToQVariant(vb);

auto vb_back = qt::QVariantToValue(qbin);
EXPECT_TRUE(vb_back.isBinary());
EXPECT_EQ(vb_back.asBinary().size(), 3);
EXPECT_EQ(vb_back.asBinary()[2], static_cast<uint8_t>(0xFF));

// datetime
auto now = std::chrono::system_clock::now();
Value vd(now);
QVariant qdt = qt::ValueToQVariant(vd);
auto vd_back = qt::QVariantToValue(qdt);
EXPECT_TRUE(vd_back.isDateTime());
}

TEST(QtItemTools, ForEachChild_HandlesRepeaterChildren) {
QmlTestFixture fixture;
const char* qml = R"qml(
import QtQuick 2.15
Item {
    Repeater {
        id: rep
        objectName: "rep"
        model: 3
        delegate: Item { objectName: "repChild" }
    }
}
)qml";

QQuickItem* root = fixture.createItem(qml);
ASSERT_NE(root, nullptr);

QObject* repeater = root->findChild<QObject*>("rep");
ASSERT_NE(repeater, nullptr);

int count = 0;
qt::ForEachChild(repeater, [&](QObject* child) -> bool {
    // We expect each child to be an Item (QQuickItem) created by the repeater delegate
    EXPECT_TRUE(qobject_cast<QQuickItem*>(child) != nullptr);
    count++;
    return true; // continue
});

EXPECT_EQ(count, 3);
}

TEST(QtItemTools, CanAttachQSignalSpyAndDetectSignal) {
QmlTestFixture fixture;
const char* qml = R"qml(
import QtQuick 2.15
Item {
    Item {
        objectName: "emitter"
        signal ping()
    }
}
)qml";

QQuickItem* root = fixture.createItem(qml);
ASSERT_NE(root, nullptr);

QObject* emitter = root->findChild<QObject*>("emitter");
ASSERT_NE(emitter, nullptr);

QSignalSpy spy(emitter, SIGNAL(ping()));
EXPECT_EQ(spy.count(), 0);

QMetaObject::invokeMethod(emitter, "ping");
EXPECT_EQ(spy.count(), 1);
}
