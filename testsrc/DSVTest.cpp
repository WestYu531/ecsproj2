#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(DSVReader, EmptyTest) {
    auto Source = std::make_shared<CStringDataSource>("");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    
    EXPECT_TRUE(Reader.End());
    EXPECT_FALSE(Reader.ReadRow(Row));
}

TEST(DSVReader, SimpleTest) {
    auto Source = std::make_shared<CStringDataSource>("a,b,c\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    
    EXPECT_FALSE(Reader.End());
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 3);
    EXPECT_EQ(Row[0], "a");
    EXPECT_EQ(Row[1], "b");
    EXPECT_EQ(Row[2], "c");
    EXPECT_TRUE(Reader.End());
}

TEST(DSVReader, QuotedTest) {
    auto Source = std::make_shared<CStringDataSource>("\"a,b\",\"c\"\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 2);
    EXPECT_EQ(Row[0], "a,b");
    EXPECT_EQ(Row[1], "c");
}

TEST(DSVReader, EscapedQuoteTest) {
    auto Source = std::make_shared<CStringDataSource>("\"a\"\"b\",c\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 2);
    EXPECT_EQ(Row[0], "a\"b");
    EXPECT_EQ(Row[1], "c");
}

TEST(DSVReader, MultilineTest) {
    auto Source = std::make_shared<CStringDataSource>("a,b\nc,d\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 2);
    EXPECT_EQ(Row[0], "a");
    EXPECT_EQ(Row[1], "b");
    
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 2);
    EXPECT_EQ(Row[0], "c");
    EXPECT_EQ(Row[1], "d");
}

TEST(DSVWriter, EmptyTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    std::vector<std::string> Row;
    
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "\n");
}

TEST(DSVWriter, SimpleTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    std::vector<std::string> Row = {"a", "b", "c"};
    
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "a,b,c\n");
}

TEST(DSVWriter, QuoteNeededTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    std::vector<std::string> Row = {"a,b", "c"};
    
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "\"a,b\",c\n");
}

TEST(DSVWriter, QuoteAllTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',', true);
    std::vector<std::string> Row = {"a", "b", "c"};
    
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "\"a\",\"b\",\"c\"\n");
}

TEST(DSVWriter, EscapeQuoteTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    std::vector<std::string> Row = {"a\"b", "c"};
    
    EXPECT_TRUE(Writer.WriteRow(Row));
    EXPECT_EQ(Sink->String(), "\"a\"\"b\",c\n");
}

TEST(DSVWriter, MultilineTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter Writer(Sink, ',');
    
    std::vector<std::string> Row1 = {"a", "b"};
    std::vector<std::string> Row2 = {"c", "d"};
    
    EXPECT_TRUE(Writer.WriteRow(Row1));
    EXPECT_TRUE(Writer.WriteRow(Row2));
    EXPECT_EQ(Sink->String(), "a,b\nc,d\n");
}

TEST(DSVReader, QuoteEscapeTest) {
    auto Source = std::make_shared<CStringDataSource>("\"My name is \"\"Bob\"\"!\",3.3\n");
    CDSVReader Reader(Source, ',');
    std::vector<std::string> Row;
    
    EXPECT_TRUE(Reader.ReadRow(Row));
    ASSERT_EQ(Row.size(), 2);
    EXPECT_EQ(Row[0], "My name is \"Bob\"!");
    EXPECT_EQ(Row[1], "3.3");
}
