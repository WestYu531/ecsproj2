# XMLReader Documentation

## Overview
The CXMLReader class provides functionality to read and parse XML documents using the Expat library. It supports all standard XML features including elements, attributes, and character data.

## Class Definition
```cpp
class CXMLReader {
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
        
    public:
        CXMLReader(std::shared_ptr<CDataSource> src);
        ~CXMLReader();
        
        bool End() const;
        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
};
```

## Constructor
```cpp
CXMLReader(std::shared_ptr<CDataSource> src)
```

Parameters:
    - src: A shared pointer to a CDataSource object providing the XML input

## Member Functions

### End()
```cpp
bool End() const
```

Returns:
    - true if all XML entities have been read
    - false if there are still entities to be read

### ReadEntity()
```cpp
bool ReadEntity(SXMLEntity &entity, bool skipcdata = false)
```

Parameters:
    - entity: Reference to an SXMLEntity to store the read entity
    - skipcdata: If true, skips character data entities

Returns:
    - true if an entity was successfully read
    - false if no more entities could be read or an error occurred

## XML Entity Types
The reader supports four types of XML entities:
```cpp
enum class EType {
    StartElement,     // Opening tag: <tag>
    EndElement,      // Closing tag: </tag>
    CharData,        // Text content
    CompleteElement  // Self-closing tag: <tag/>
};
```

## XML Entity Structure
```cpp
struct SXMLEntity {
    EType DType;                                    // Type of the entity
    std::string DNameData;                         // Tag name or character data
    std::vector<std::pair<std::string,std::string>> DAttributes;  // Attributes
};
```

## Usage Example
```cpp
auto Source = std::make_shared<CStringDataSource>(
    "<root attr=\"value\">"
    "    <child>text</child>"
    "</root>"
);
CXMLReader Reader(Source);
SXMLEntity Entity;

while(!Reader.End()) {
    if(Reader.ReadEntity(Entity)) {
        switch(Entity.DType) {
            case SXMLEntity::EType::StartElement:
                // Process start tag
                break;
            case SXMLEntity::EType::EndElement:
                // Process end tag
                break;
            case SXMLEntity::EType::CharData:
                // Process text content
                break;
            case SXMLEntity::EType::CompleteElement:
                // Process self-closing tag
                break;
        }
    }
}
```

## Error Handling
- ReadEntity() returns false if:
    - The end of input is reached
    - XML parsing error occurs
    - Memory allocation fails
- The reader stops parsing after the first error
- Malformed XML results in immediate failure

## Special Features
- Automatic handling of XML namespaces
- Support for XML special characters (&lt;, &gt;, &amp;, etc.)
- Optional skipping of whitespace-only character data
- Proper handling of CDATA sections
- Support for XML comments (ignored)

## Performance Considerations
- Uses Expat for efficient XML parsing
- Streaming parser, minimal memory overhead
- Entity queue prevents unnecessary parsing
- No DOM tree construction
- Suitable for large XML documents

## Best Practices
- Always check ReadEntity() return value
- Process entities in the order they are read
- Use skipcdata=true to ignore whitespace
- Keep track of element nesting if needed
- Release the reader when done to free resources 