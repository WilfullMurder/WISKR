#include <gtest/gtest.h>
#include <wiskr/data/item_path_component.h>
#include <wiskr/data/item_path.h>
#include <wiskr/data/item_position.h>
#include <wiskr/data/geometry.h>

using namespace whisker;

TEST(ComponentParsesPropertySelectorFromDotPrefix, ReturnsDotPrefixedString) {
path::Component c(".prop");
EXPECT_EQ(c.string(), ".prop");
}

TEST(ComponentParsesTypeSelectorFromHashPrefix, ReturnsHashPrefixedString) {
path::Component c("#Button");
EXPECT_EQ(c.string(), "#Button");
}

TEST(ComponentParsesValueSelectorFromQuotedString, PreservesQuotesInString) {
path::Component c("\"hello\"");
EXPECT_EQ(c.string(), "\"hello\"");
}

TEST(ComponentParsesPropertyValueSelectorFromParentheses, ReturnsParenthesizedKeyValue) {
path::Component c("(key=value)");
EXPECT_EQ(c.string(), "(key=value)");
}

TEST(ComponentParsesPlainNameSelector, ReturnsNameUnchanged) {
path::Component c("nodeName");
EXPECT_EQ(c.string(), "nodeName");
}

TEST(ComponentHandlesEmptyRawString, ReturnsEmptyString) {
path::Component c("");
EXPECT_EQ(c.string(), "");
}

TEST(ItemPathConstructedFromComponentsProducesCorrectString, JoinsComponentStringsWithSlashes) {
std::vector<path::Component> comps = { path::Component("root"), path::Component("#Type"), path::Component(".prop") };
ItemPath p(comps);
EXPECT_EQ(p.length(), 3u);
EXPECT_EQ(p.string(), "root/#Type/.prop");
}

TEST(ItemPathRootReturnsFirstComponentString, RootMatchesFirstComponent) {
std::vector<path::Component> comps = { path::Component("first"), path::Component("second") };
ItemPath p(comps);
EXPECT_EQ(p.root().string(), "first");
}

TEST(ItemPathSubPathReturnsSuffixStartingAtOffset, SubPathContainsRemainingComponents) {
std::vector<path::Component> comps = { path::Component("a"), path::Component("b"), path::Component("c") };
ItemPath p(comps);
ItemPath sub = p.subPath(1);
EXPECT_EQ(sub.length(), 2u);
EXPECT_EQ(sub.string(), "b/c");
}

TEST(ItemPositionComputesAbsolutePositionFromProportionAndOffset, ProducesExpectedCoordinates) {
ItemPath path;
Point proportion{0.25, 0.75};
Point offset{2.0, -3.0};
ItemPosition pos(path, proportion, offset);
Size size{100.0, 200.0};
Point result = pos.positionForItemSize(size);
EXPECT_DOUBLE_EQ(result.x, 25.0 + 2.0);
EXPECT_DOUBLE_EQ(result.y, 150.0 - 3.0);
}

TEST(ItemPositionDefaultProportionIsCenterWhenNotProvided, ComputesCenterOfItem) {
ItemPath path;
ItemPosition pos(path); // uses default proportion {0.5,0.5} and offset {0,0}
Size size{10.0, 20.0};
Point result = pos.positionForItemSize(size);
EXPECT_DOUBLE_EQ(result.x, 5.0);
EXPECT_DOUBLE_EQ(result.y, 10.0);
}