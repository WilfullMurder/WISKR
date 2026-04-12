#include <gtest/gtest.h>
#include <wiskr/data/item_path.h>
#include <wiskr/data/item_path_component.h>
#include <wiskr/data/item_position.h>

TEST(ItemPathConstructedFromEmptyStringHasZeroLength, ReturnsZeroLengthAndEmptyString) {
whisker::ItemPath p(std::string(""));
EXPECT_EQ(p.length(), 0u);
EXPECT_EQ(p.string(), "");
}

TEST(ItemPathRootThrowsOnEmptyPath, ThrowsOutOfRange) {
whisker::ItemPath p;
EXPECT_THROW(p.root(), std::out_of_range);
}

TEST(ItemPathSubPathOffsetBeyondSizeReturnsEmptyPath, ReturnsEmptyPath) {
std::vector<whisker::path::Component> comps = { whisker::path::Component("a") };
whisker::ItemPath p(comps);
auto sub = p.subPath(5);
EXPECT_EQ(sub.length(), 0u);
EXPECT_EQ(sub.string(), "");
}

TEST(ItemPathSubPathOffsetZeroReturnsSamePathString, ReturnsIdenticalContents) {
std::vector<whisker::path::Component> comps = { whisker::path::Component("a"), whisker::path::Component("b") };
whisker::ItemPath p(comps);
auto sub = p.subPath(0);
EXPECT_EQ(sub.length(), p.length());
EXPECT_EQ(sub.string(), p.string());
}

TEST(ItemPathConstructedFromCStringParsesAndFormats, RoundTripsNormalizedString) {
const char* raw = "root/#Type/.prop";
whisker::ItemPath p(raw);
EXPECT_EQ(p.length(), 3u);
EXPECT_EQ(p.string(), std::string(raw));
}

TEST(ComponentPreservesVariousRawForms, ReturnsOriginalRepresentation) {
whisker::path::Component c1("nodeName");
whisker::path::Component c2(".prop");
whisker::path::Component c3("#Button");
whisker::path::Component c4("\"value\"");
whisker::path::Component c5("(key=value)");
EXPECT_EQ(c1.string(), "nodeName");
EXPECT_EQ(c2.string(), ".prop");
EXPECT_EQ(c3.string(), "#Button");
EXPECT_EQ(c4.string(), "\"value\"");
EXPECT_EQ(c5.string(), "(key=value)");
}

TEST(ItemPositionComputesWithNegativeOffsetAndOutOfRangeProportion, ProducesExpectedCoordinates) {
whisker::ItemPath path;
whisker::Point proportion{2.0, -1.0};
whisker::Point offset{-5.0, 3.0};
whisker::ItemPosition pos(path, proportion, offset);
whisker::Size size{10.0, 10.0};
auto result = pos.positionForItemSize(size);
EXPECT_DOUBLE_EQ(result.x, 2.0 * 10.0 - 5.0);
EXPECT_DOUBLE_EQ(result.y, -1.0 * 10.0 + 3.0);
}

TEST(ItemPositionDefaultProportionAndOffsetProduceCenter, ComputesCenterWhenDefaultsUsed) {
whisker::ItemPath path;
whisker::ItemPosition pos(path);
whisker::Size size{20.0, 40.0};
auto result = pos.positionForItemSize(size);
EXPECT_DOUBLE_EQ(result.x, 10.0);
EXPECT_DOUBLE_EQ(result.y, 20.0);
}