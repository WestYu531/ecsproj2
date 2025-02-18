# XMLWriter Documentation

## Overview
The CXMLWriter class provides functionality to write XML documents with proper formatting. It handles element nesting, attributes, and character data, with automatic escaping of special characters.

## Class Definition
```cpp
class CXMLWriter {
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    
    public:
        CXMLWriter(std::shared_ptr<CDataSink> sink);
        ~CXMLWriter();
        
        bool WriteEntity(const SXMLEntity &entity);
        bool Flush();
};
```

## Constructor
```cpp
CXMLWriter(std::shared_ptr<CDataSink> sink)
```

Parameters:
    - sink: A shared pointer to a CDataSink object for output

## Member Functions

### WriteEntity()
```cpp
bool WriteEntity(const SXMLEntity &entity)
```

Parameters:
    - entity: The XML entity to write

Returns:
    - true if the entity was successfully written
    - false if an error occurred

### Flush()
```cpp
bool Flush()
```

Returns:
    - true if all pending end tags were successfully written
    - false if an error occurred

## XML Entity Types
The writer supports four types of XML entities:
```cpp
enum class EType {
    StartElement,     // Opening tag: <tag>
    EndElement,      // Closing tag: </tag>
    CharData,        // Text content
    CompleteElement  // Self-closing tag: <tag/>
};
```

## Writing Examples

### Basic Elements
```cpp
SXMLEntity Entity;
Entity.DType = SXMLEntity::EType::StartElement;
Entity.DNameData = "root";
Writer.WriteEntity(Entity);  // Outputs: <root>

Entity.DType = SXMLEntity::EType::EndElement;
Writer.WriteEntity(Entity);  // Outputs: </root>
```

### Elements with Attributes
```cpp
SXMLEntity Entity;
Entity.DType = SXMLEntity::EType::StartElement;
Entity.DNameData = "element";
Entity.DAttributes.push_back({"attr", "value"});
Writer.WriteEntity(Entity);  // Outputs: <element attr="value">
```

### Character Data
```cpp
SXMLEntity Entity;
Entity.DType = SXMLEntity::EType::CharData;
Entity.DNameData = "Some text & special chars";
Writer.WriteEntity(Entity);  // Outputs: Some text &amp; special chars
```

### Complete Elements
```cpp
SXMLEntity Entity;
Entity.DType = SXMLEntity::EType::CompleteElement;
Entity.DNameData = "empty";
Writer.WriteEntity(Entity);  // Outputs: <empty/>
```

## Special Character Handling
- < becomes \&lt;
- \> becomes \&gt;
- & becomes \&amp;
- " becomes \&quot;
- ' becomes \&apos;

## Usage Example
```cpp
auto Sink = std::make_shared<CStringDataSink>();
CXMLWriter Writer(Sink);
SXMLEntity Entity;

// Write root element with attribute
Entity.DType = SXMLEntity::EType::StartElement;
Entity.DNameData = "root";
Entity.DAttributes.push_back({"version", "1.0"});
Writer.WriteEntity(Entity);

// Write child element with content
Entity.DType = SXMLEntity::EType::StartElement;
Entity.DNameData = "child";
Entity.DAttributes.clear();
Writer.WriteEntity(Entity);

Entity.DType = SXMLEntity::EType::CharData;
Entity.DNameData = "Hello, World!";
Writer.WriteEntity(Entity);

Entity.DType = SXMLEntity::EType::EndElement;
Entity.DNameData = "child";
Writer.WriteEntity(Entity);

// Close root element
Entity.DNameData = "root";
Writer.WriteEntity(Entity);

Writer.Flush();
```

## Error Handling
- WriteEntity() returns false if:
    - Memory allocation fails
    - The underlying sink fails to accept data
    - Invalid entity type or data
- Flush() returns false if:
    - Unable to write remaining end tags
    - The underlying sink fails

## Performance Considerations
- Maintains element stack for proper nesting
- Immediate writing of entities
- No XML validation performed
- Memory usage proportional to nesting depth
- Efficient handling of large documents

## Best Practices
- Always match StartElement with EndElement
- Call Flush() when finished writing
- Check return values for error detection
- Use CompleteElement for empty elements
- Properly escape special characters in attributes 