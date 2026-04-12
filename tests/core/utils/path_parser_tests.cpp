#include <gtest/gtest.h>
#include <wiskr/utils/path_parser.h>

TEST(PathParserParsesSimplePathIntoComponents, ReturnsEachSegment) {
auto comps = whisker::utils::ParsePathFrom("a/b/c");
ASSERT_EQ(comps.size(), 3u);
EXPECT_EQ(comps[0], "a");
EXPECT_EQ(comps[1], "b");
EXPECT_EQ(comps[2], "c");
}

TEST(PathParserHandlesEscapedSlashInComponent, EscapedSlashProducesLiteralSlash) {
auto comps = whisker::utils::ParsePathFrom("a\\/b/c");
ASSERT_EQ(comps.size(), 2u);
EXPECT_EQ(comps[0], "a/b");
EXPECT_EQ(comps[1], "c");
}

TEST(PathParserHandlesEscapedBackslashInComponent, EscapedBackslashProducesLiteralBackslash) {
auto comps = whisker::utils::ParsePathFrom("a\\\\b/c");
ASSERT_EQ(comps.size(), 2u);
EXPECT_EQ(comps[0], "a\\b");
EXPECT_EQ(comps[1], "c");
}

TEST(PathParserIgnoresEmptyComponentsFromConsecutiveOrBoundarySlashes, ReturnsOnlyNonEmptyComponents) {
auto comps = whisker::utils::ParsePathFrom("/a//b/");
ASSERT_EQ(comps.size(), 2u);
EXPECT_EQ(comps[0], "a");
EXPECT_EQ(comps[1], "b");
}

TEST(PathParserEmptyInputReturnsNoComponents, ReturnsEmptyVector) {
auto comps = whisker::utils::ParsePathFrom("");
EXPECT_TRUE(comps.empty());
}

TEST(PathFormatterJoinsSimpleComponentsWithSlashes, ProducesJoinedPath) {
std::vector<std::string> components = {"a", "b", "c"};
EXPECT_EQ(whisker::utils::FormatPath(components), "a/b/c");
}

TEST(PathFormatterEscapesSlashesInComponents, EscapesSlashWithBackslash) {
std::vector<std::string> components = {"a/b", "c"};
EXPECT_EQ(whisker::utils::FormatPath(components), "a\\/b/c");
}

TEST(PathFormatterEscapesBackslashesInComponents, DoublesBackslashes) {
std::vector<std::string> components = {"a\\b", "c"};
EXPECT_EQ(whisker::utils::FormatPath(components), "a\\\\b/c");
}

TEST(PathFormatterHandlesEmptyComponentList, ReturnsEmptyString) {
std::vector<std::string> components;
EXPECT_EQ(whisker::utils::FormatPath(components), "");
}
