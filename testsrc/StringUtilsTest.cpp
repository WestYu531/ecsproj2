#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtils, SliceTest){
    EXPECT_EQ(StringUtils::Slice("Hello", 0), "Hello");
    EXPECT_EQ(StringUtils::Slice("Hello", 1), "ello");
    EXPECT_EQ(StringUtils::Slice("Hello", 0, 3), "Hel");
    EXPECT_EQ(StringUtils::Slice("Hello", -2), "lo");
    EXPECT_EQ(StringUtils::Slice("Hello", 1, -1), "ell");
}

TEST(StringUtils, CapitalizeTest){
    EXPECT_EQ(StringUtils::Capitalize("hello"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize("HELLO"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize("hELLO"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize(""), "");
}

TEST(StringUtils, UpperTest){
    EXPECT_EQ(StringUtils::Upper("hello"), "HELLO");
    EXPECT_EQ(StringUtils::Upper("HELLO"), "HELLO");
    EXPECT_EQ(StringUtils::Upper("Hello123"), "HELLO123");
    EXPECT_EQ(StringUtils::Upper(""), "");
}

TEST(StringUtils, LowerTest){
    EXPECT_EQ(StringUtils::Lower("HELLO"), "hello");
    EXPECT_EQ(StringUtils::Lower("hello"), "hello");
    EXPECT_EQ(StringUtils::Lower("HeLLo123"), "hello123");
    EXPECT_EQ(StringUtils::Lower(""), "");
}

TEST(StringUtils, StripTest){
    EXPECT_EQ(StringUtils::Strip("  hello  "), "hello");
    EXPECT_EQ(StringUtils::Strip("\t\nhello\n\t"), "hello");
    EXPECT_EQ(StringUtils::Strip("hello"), "hello");
    EXPECT_EQ(StringUtils::Strip(""), "");
}

TEST(StringUtils, CenterTest){
    EXPECT_EQ(StringUtils::Center("hello", 11), "   hello   ");
    EXPECT_EQ(StringUtils::Center("hello", 10), "  hello   ");
    EXPECT_EQ(StringUtils::Center("hello", 5), "hello");
    EXPECT_EQ(StringUtils::Center("hello", 4), "hello");
}

TEST(StringUtils, LJustTest){
    EXPECT_EQ(StringUtils::LJust("hello", 8), "hello   ");
    EXPECT_EQ(StringUtils::LJust("hello", 5), "hello");
    EXPECT_EQ(StringUtils::LJust("hello", 4), "hello");
}

TEST(StringUtils, RJustTest){
    EXPECT_EQ(StringUtils::RJust("hello", 8), "   hello");
    EXPECT_EQ(StringUtils::RJust("hello", 5), "hello");
    EXPECT_EQ(StringUtils::RJust("hello", 4), "hello");
}

TEST(StringUtils, ReplaceTest){
    EXPECT_EQ(StringUtils::Replace("hello world", "world", "there"), "hello there");
    EXPECT_EQ(StringUtils::Replace("hello hello", "hello", "hi"), "hi hi");
    EXPECT_EQ(StringUtils::Replace("hello", "world", "there"), "hello");
}

TEST(StringUtils, SplitTest){
    std::vector<std::string> expected1 = {"hello", "world"};
    EXPECT_EQ(StringUtils::Split("hello world"), expected1);
    
    std::vector<std::string> expected2 = {"hello", "world"};
    EXPECT_EQ(StringUtils::Split("hello,world", ","), expected2);
    
    std::vector<std::string> expected3 = {"", "hello", "", "world", ""};
    EXPECT_EQ(StringUtils::Split(",hello,,world,", ","), expected3);
}

TEST(StringUtils, JoinTest){
    std::vector<std::string> input = {"hello", "world"};
    EXPECT_EQ(StringUtils::Join(" ", input), "hello world");
    EXPECT_EQ(StringUtils::Join(",", input), "hello,world");
    EXPECT_EQ(StringUtils::Join("", input), "helloworld");
}

TEST(StringUtils, ExpandTabsTest){
    EXPECT_EQ(StringUtils::ExpandTabs("hello\tworld"), "hello   world");
    EXPECT_EQ(StringUtils::ExpandTabs("\thello"), "    hello");
    EXPECT_EQ(StringUtils::ExpandTabs("hello\t\tworld"), "hello       world");
}

TEST(StringUtils, EditDistanceTest){
    EXPECT_EQ(StringUtils::EditDistance("hello", "hello"), 0);
    EXPECT_EQ(StringUtils::EditDistance("hello", "helo"), 1);
    EXPECT_EQ(StringUtils::EditDistance("hello", "world"), 4);
    EXPECT_EQ(StringUtils::EditDistance("hello", "HELLO", true), 0);
} 