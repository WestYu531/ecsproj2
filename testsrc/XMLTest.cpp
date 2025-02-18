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

TEST(XMLWriter, SpecialCharTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(Sink);
    SXMLEntity Entity;
    
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "e";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    Entity.DType = SXMLEntity::EType::CharData;
    Entity.DNameData = "&\"'<>";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    Entity.DType = SXMLEntity::EType::EndElement;
    Entity.DNameData = "e";
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    std::string Expected = "<e>&amp;&quot;&apos;&lt;&gt;</e>";
    EXPECT_EQ(Sink->String(), Expected);
}

TEST(XMLWriter, OSMFormatTest) {
    auto Sink = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(Sink);
    SXMLEntity Entity;
    
    // Write OSM start element
    Entity.DType = SXMLEntity::EType::StartElement;
    Entity.DNameData = "osm";
    Entity.DAttributes.push_back(std::make_pair("version", "0.6"));
    Entity.DAttributes.push_back(std::make_pair("generator", "osmconvert 0.8.5"));
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    // Write first node
    Entity.DType = SXMLEntity::EType::CompleteElement;
    Entity.DNameData = "node";
    Entity.DAttributes.clear();
    Entity.DAttributes.push_back(std::make_pair("id", "62208369"));
    Entity.DAttributes.push_back(std::make_pair("lat", "38.5178523"));
    Entity.DAttributes.push_back(std::make_pair("lon", "-121.7712408"));
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    // Write second node
    Entity.DAttributes.clear();
    Entity.DAttributes.push_back(std::make_pair("id", "62209104"));
    Entity.DAttributes.push_back(std::make_pair("lat", "38.535052"));
    Entity.DAttributes.push_back(std::make_pair("lon", "-121.7408606"));
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    // Write OSM end element
    Entity.DType = SXMLEntity::EType::EndElement;
    Entity.DNameData = "osm";
    Entity.DAttributes.clear();
    EXPECT_TRUE(Writer.WriteEntity(Entity));
    
    std::string Expected = "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                          "<node id=\"62208369\" lat=\"38.5178523\" lon=\"-121.7712408\"/>"
                          "<node id=\"62209104\" lat=\"38.535052\" lon=\"-121.7408606\"/>"
                          "</osm>";
    EXPECT_EQ(Sink->String(), Expected);
}
