# DSVReader Documentation

## Overview
The CDSVReader class provides functionality to read delimiter-separated value (DSV) files. It supports various delimiters and handles quoted values properly.

## Class Definition
```cpp
class CDSVReader {
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    
    public:
        CDSVReader(std::shared_ptr<CDataSource> src, char delimiter);
        ~CDSVReader();
        
        bool End() const;
        bool ReadRow(std::vector<std::string> &row);
};
```

## Constructor
```cpp
CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
```

Parameters:
    - src: A shared pointer to a CDataSource object providing the input data
    - delimiter: The character used to separate values (if '"', uses ',' instead)

## Member Functions

### End()
```cpp
bool End() const
```

Returns:
    - true if all rows have been read from the DSV
    - false if there are still rows to be read

### ReadRow()
```cpp
bool ReadRow(std::vector<std::string> &row)
```

Parameters:
    - row: Vector to store the fields of the current row

Returns:
    - true if a row was successfully read
    - false if no more rows could be read

## Special Cases

### Quoted Fields
- Fields containing the delimiter must be quoted
- Fields containing double quotes must be quoted
- Fields containing newlines must be quoted
- Double quotes within quoted fields must be escaped with another double quote

### Example:
```cpp
Input: "Hello, World","This ""quote"" is escaped",simple
Result: ["Hello, World"]["This "quote" is escaped"]["simple"]
```

### Empty Fields
- Empty fields are represented as empty strings
- Consecutive delimiters create empty fields
- A line ending with a delimiter creates a trailing empty field

### Example:
```cpp
Input: a,,b,
Result: ["a"][""]["b"][""]
```

## Usage Example
```cpp
auto Source = std::make_shared<CStringDataSource>("a,b,c\nd,e,f");
CDSVReader Reader(Source, ',');
std::vector<std::string> Row;

while(!Reader.End()) {
    if(Reader.ReadRow(Row)) {
        // Process Row...
    }
}
```

## Error Handling
- ReadRow() returns false if:
    - The end of input is reached
    - A malformed quoted field is encountered
    - Memory allocation fails
- The reader is designed to be robust against malformed input

## Performance Considerations
- Input is processed character by character
- No internal buffering beyond the current row
- Memory usage is proportional to the size of the current row
- String copies are minimized where possible 