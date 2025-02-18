#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include <algorithm>

TEST(XMLReader, EmptyTest) {
    auto Source = std::make_shared<CStringDataSource>("");
    CXMLReader Reader(Source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.End());
    EXPECT_FALSE(Reader.ReadEntity(Entity));
}

TEST(XMLReader, SimpleElementTest) {
    auto Source = std::make_shared<CStringDataSource>("<element></element>");
    CXMLReader Reader(Source);
    SXMLEntity Entity;
    
    EXPECT_FALSE(Reader.End());
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "element");
    EXPECT_TRUE(Entity.DAttributes.empty());
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "element");
    
    EXPECT_TRUE(Reader.End());
    EXPECT_FALSE(Reader.ReadEntity(Entity));
}

TEST(XMLReader, AttributeTest) {
    auto Source = std::make_shared<CStringDataSource>("<element attr1=\"value1\" attr2=\"value2\"></element>");
    CXMLReader Reader(Source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "element");
    ASSERT_EQ(Entity.DAttributes.size(), 2);
    EXPECT_EQ(Entity.DAttributes[0].first, "attr1");
    EXPECT_EQ(Entity.DAttributes[0].second, "value1");
    EXPECT_EQ(Entity.DAttributes[1].first, "attr2");
    EXPECT_EQ(Entity.DAttributes[1].second, "value2");
}

TEST(XMLReader, NestedElementTest) {
    auto Source = std::make_shared<CStringDataSource>("<root><child>text</child></root>");
    CXMLReader Reader(Source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "root");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "child");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "text");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "child");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "root");
}

TEST(XMLReader, SpecialCharTest) {
    auto Source = std::make_shared<CStringDataSource>("<e>&amp;&quot;&apos;&lt;&gt;</e>");
    CXMLReader Reader(Source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "e");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "&\"'<>");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "e");
}

TEST(XMLReader, QuoteTest) {
    auto Source = std::make_shared<CStringDataSource>("<element attr=\"value with &quot;quotes&quot;\">Text with \"quotes\"</element>");
    CXMLReader Reader(Source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "element");
    ASSERT_EQ(Entity.DAttributes.size(), 1);
    EXPECT_EQ(Entity.DAttributes[0].first, "attr");
    EXPECT_EQ(Entity.DAttributes[0].second, "value with \"quotes\"");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "Text with \"quotes\"");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "element");
}

TEST(XMLWriter, EmptyTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(Sink);
    
    EXPECT_TRUE(Writer.Flush());
    EXPECT_TRUE(Sink->String().empty());
}

TEST(XMLWriter, SimpleElementTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(Sink);
    SXMLEntity Entity;
    
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "element";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    Entity.DType = SXMLEntity::EType::EndElement;
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    std::string Expected = "<element></element>";
    EXPECT_EQ(Sink->String(), Expected);
}

TEST(XMLWriter, AttributeTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(Sink);
    SXMLEntity Entity;
    
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "element";
    Entity.DAttributes.push_back(std::make_pair("attr1", "value1"));
    Entity.DAttributes.push_back(std::make_pair("attr2", "value2"));
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    Entity.DType = SXMLEntity::EType::EndElement;
    Entity.DAttributes.clear();
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    std::string Expected = "<element attr1=\"value1\" attr2=\"value2\"></element>";
    EXPECT_EQ(Sink->String(), Expected);
}

TEST(XMLWriter, CharDataTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(Sink);
    SXMLEntity Entity;
    
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "element";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    Entity.DType = SXMLEntity::EType::CharData;
    Entity.DNameData = "Some text with <special> &chars";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    Entity.DType = SXMLEntity::EType::EndElement;
    Entity.DNameData = "element";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    std::string Expected = "<element>Some text with &lt;special&gt; &amp;chars</element>";
    EXPECT_EQ(Sink->String(), Expected);
}

TEST(XMLWriter, CompleteElementTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(Sink);
    SXMLEntity Entity;
    
    Entity.DType = SXMLEntity::EType::CompleteElement;
    Entity.DNameData = "element";
    Entity.DAttributes.push_back(std::make_pair("attr", "value"));
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    std::string Expected = "<element attr=\"value\"/>";
    EXPECT_EQ(Sink->String(), Expected);
} 