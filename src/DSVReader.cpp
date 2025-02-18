#include "DSVReader.h"
#include <sstream>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DDataSource;
    char DDelimiter;
    
    SImplementation(std::shared_ptr<CDataSource> src, char delimiter) 
        : DDataSource(src), DDelimiter(delimiter == '"' ? ',' : delimiter) {
    }
    
    bool End() const {
        return DDataSource->End();
    }
    
    bool ReadRow(std::vector<std::string> &row) {
        row.clear();
        if(DDataSource->End()){
            return false;
        }
        
        std::string currentField;
        bool inQuotes = false;
        bool hasData = false;
        
        while(true){
            char ch;
            if(!DDataSource->Get(ch)){
                if(hasData || !currentField.empty()){
                    row.push_back(currentField);
                    return true;
                }
                return hasData;
            }
            
            hasData = true;
            
            if(ch == '"'){
                if(inQuotes){
                    // Check for escaped quote
                    char nextCh;
                    if(DDataSource->Peek(nextCh) && nextCh == '"'){
                        DDataSource->Get(nextCh); // Consume the second quote
                        currentField += '"'; // Only add one quote
                    }
                    else{
                        inQuotes = false;
                    }
                }
                else if(currentField.empty()){
                    inQuotes = true;
                }
                else{
                    currentField += ch;
                }
            }
            else if(ch == '\n'){
                if(inQuotes){
                    currentField += ch;
                }
                else{
                    row.push_back(currentField);
                    return true;
                }
            }
            else if(ch == DDelimiter){
                if(inQuotes){
                    currentField += ch;
                }
                else{
                    row.push_back(currentField);
                    currentField.clear();
                }
            }
            else{
                currentField += ch;
            }
        }
        
        return false;
    }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
}

CDSVReader::~CDSVReader(){
}

bool CDSVReader::End() const{
    return DImplementation->End();
}

bool CDSVReader::ReadRow(std::vector<std::string> &row){
    return DImplementation->ReadRow(row);
} 