# DSVWriter Documentation

## Overview
The CDSVWriter class provides functionality to write delimiter-separated value (DSV) files. It handles quoting and escaping of special characters automatically.

## Class Definition
```cpp
class CDSVWriter {
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    
    public:
        CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall = false);
        ~CDSVWriter();
        
        bool WriteRow(const std::vector<std::string> &row);
};
```

## Constructor
```cpp
CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall = false)
```

Parameters:
    - sink: A shared pointer to a CDataSink object for output
    - delimiter: The character used to separate values (if '"', uses ',' instead)
    - quoteall: If true, quotes all fields regardless of content

## Member Functions

### WriteRow()
```cpp
bool WriteRow(const std::vector<std::string> &row)
```

Parameters:
    - row: Vector containing the fields to write as a single row

Returns:
    - true if the row was successfully written
    - false if an error occurred

## Automatic Quoting
Fields are automatically quoted if they contain any of:
    - The delimiter character
    - Double quotes
    - Newlines
    - Leading or trailing whitespace

When quoteall is true, all fields are quoted regardless of content.

## Escaping Rules
- Double quotes within fields are escaped by doubling them
- The delimiter is written as-is outside of quoted fields
- A newline character is appended after each row

### Examples:
```cpp
// Basic writing
Input: ["a", "b", "c"]
Output: a,b,c

// Automatic quoting
Input: ["Hello, World", "Simple"]
Output: "Hello, World",Simple

// Quote escaping
Input: ["He said ""Hello"""]
Output: "He said ""Hello"""

// Quote all mode
Input: ["a", "b", "c"] (with quoteall=true)
Output: "a","b","c"
```

## Usage Example
```cpp
auto Sink = std::make_shared<CStringDataSink>();
CDSVWriter Writer(Sink, ',');
std::vector<std::string> Row = {"Hello, World", "Simple", "Test"};

if(Writer.WriteRow(Row)) {
    // Row written successfully
    // Output will be: "Hello, World",Simple,Test
}
```

## Error Handling
- WriteRow() returns false if:
    - Memory allocation fails
    - The underlying sink fails to accept data
- The writer is designed to be exception-safe

## Performance Considerations
- Output is written character by character
- No internal buffering
- Memory usage is proportional to the size of the current row
- String copies are avoided where possible
- Quote analysis is performed once per field

## Best Practices
- Use quoteall=true if consistency is more important than space efficiency
- Check the return value of WriteRow() for error detection
- Close or flush the underlying sink when writing is complete
- Avoid extremely large rows that could cause memory issues 