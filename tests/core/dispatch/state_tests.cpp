#include <gtest/gtest.h>
#include <wiskr/dispatch/state.h>

using namespace whisker;

TEST(StateReportsNoErrorsInitially, returns_no_errors_and_empty_summary) {
State s;
EXPECT_FALSE(s.hasErrors());
EXPECT_TRUE(s.errors().empty());
EXPECT_EQ(s.errorSummary(), "");
}

TEST(StateReportsErrorsAfterReport, has_errors_and_contains_reported_message) {
State s;
s.reportError("first error");
EXPECT_TRUE(s.hasErrors());
auto errs = s.errors();
ASSERT_EQ(errs.size(), 1u);
EXPECT_EQ(errs[0], "first error");
EXPECT_EQ(s.errorSummary(), "first error");
}

TEST(StateAccumulatesMultipleErrors, preserves_order_and_counts) {
State s;
s.reportError("err1");
s.reportError("err2");
s.reportError("err3");
EXPECT_TRUE(s.hasErrors());
auto errs = s.errors();
ASSERT_EQ(errs.size(), 3u);
EXPECT_EQ(errs[0], "err1");
EXPECT_EQ(errs[1], "err2");
EXPECT_EQ(errs[2], "err3");
EXPECT_EQ(s.errorSummary(), "err1\nerr2\nerr3");
}

TEST(StateErrorSummaryWithEmptyMessage, records_empty_error_but_summary_empty) {
State s;
s.reportError("");
EXPECT_TRUE(s.hasErrors());
auto errs = s.errors();
ASSERT_EQ(errs.size(), 1u);
EXPECT_EQ(errs[0], "");
EXPECT_EQ(s.errorSummary(), "");
}

TEST(StateErrorsAreImmutableCopy, errors_returns_copy_not_reference) {
State s;
s.reportError("x");
auto errs1 = s.errors();
errs1.push_back("mutated");
auto errs2 = s.errors();
ASSERT_EQ(errs2.size(), 1u);
EXPECT_EQ(errs2[0], "x");
}
