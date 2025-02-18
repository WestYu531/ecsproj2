#include "DSVReader.h"
#include <vector>
#include <string>
#include <memory>




CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter) {
    auto box = std::make_unique<SImplementation>(src, delimiter);

//Store the box in the pointer named DImplementation
DImplementation = std::move(box);
//DImplementation(std::make_unique<SImplementation>(src, delimiter)). It is simple but I don't understand.
}


CDSVReader::~CDSVReader() {
}

bool CDSVReader::End() const {
    return DImplementation->end;
}

bool CDSVReader::ReadRow(std::vector<std::string> &row) {
    row.clear();
    std::string temp;
    char ch;
    bool quote = false;
    while (DImplementation->DSource->Get(ch)) {
        if (ch=='"') {
            char next_char;
            if (DImplementation->DSource->Peek(next_char) && next_char == '"') {
                temp += ch;
                DImplementation->DSource->Get(ch);
            }
            else

            quote = !quote;
           
        }

       else if (ch == DImplementation->delimiter && !quote) {

        row.push_back(temp);
        temp.clear();
       }
        
        else if (ch == '\n') {
            row.push_back(temp);
            temp.clear();
            return true;
            
        }
        else {
            temp += ch;
        }
   
    }
     if (DImplementation->DSource->End()) {
         row.push_back(temp);
        DImplementation->end = true;
        return true;
        

    }
    return false;
}

