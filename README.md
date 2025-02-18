# Project 2 - DSV and XML Parser

This project implements DSV (Delimiter-Separated Values) and XML parsers in C++. It includes classes for reading and writing both DSV and XML formats, along with comprehensive unit tests.

## Components

### DSV Components
- CDSVReader: Reads delimiter-separated value files
- CDSVWriter: Writes delimiter-separated value files
- Supports custom delimiters
- Handles quoted values and escaping

### XML Components
- CXMLReader: Reads XML files using the Expat library
- CXMLWriter: Writes XML files with proper formatting
- Supports XML attributes and nested elements
- Handles character data and special characters

### Utility Components
- CDataSource: Abstract base class for data input
- CDataSink: Abstract base class for data output
- CStringDataSource: String-based implementation of CDataSource
- CStringDataSink: String-based implementation of CDataSink

## Building and Testing

### Prerequisites
- C++17 compatible compiler
- Google Test framework
- Expat XML library
- Make build system

### Build Instructions
1. Ensure all prerequisites are installed
2. Run `make` to build all components
3. Run `make test` to execute all unit tests
4. Run `make clean` to remove build artifacts

### Test Executables
- teststrutils: Tests string utility functions
- teststrdatasource: Tests string data source
- teststrdatasink: Tests string data sink
- testdsv: Tests DSV reader and writer
- testxml: Tests XML reader and writer

## Implementation Details

### DSV Format Support
- Handles comma-separated values (CSV)
- Supports custom delimiters
- Properly handles quoted fields
- Escapes double quotes within fields
- Supports empty fields and lines

### XML Format Support
- Full XML element support (start/end/complete)
- Attribute handling
- Character data support
- Special character escaping
- Proper indentation

## External Libraries Used
- Google Test (gtest) for unit testing
- Expat for XML parsing

## File Structure
- include/: Header files
- src/: Implementation files
- testsrc/: Test files
- docs/: Documentation
- Makefile: Build system

However, I have used **Generative AI** to:
- Simplify the code implementation by improving readability.
- Identify and correct syntax errors.
- Optimize logic to ensure correct handling of edge cases.

All AI-generated content has been reviewed and modified to meet project requirements.