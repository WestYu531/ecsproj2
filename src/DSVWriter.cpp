#include "DSVWriter.h"
#include <algorithm>

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> DDataSink;
    char DDelimiter;
    bool DQuoteAll;
    
    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall) 
        : DDataSink(sink), DDelimiter(delimiter == '"' ? ',' : delimiter), DQuoteAll(quoteall) {
    }
    
    bool NeedsQuoting(const std::string &str) const {
        if(DQuoteAll){
            return true;
        }
        return str.find(DDelimiter) != std::string::npos || 
               str.find('"') != std::string::npos || 
               str.find('\n') != std::string::npos;
    }
    
    bool WriteQuoted(const std::string &str){
        if(!DDataSink->Put('"')){
            return false;
        }
        
        for(char ch : str){
            if(ch == '"'){
                if(!DDataSink->Put('"')){ // Escape quote with another quote
                    return false;
                }
            }
            if(!DDataSink->Put(ch)){
                return false;
            }
        }
        
        return DDataSink->Put('"');
    }
    
    bool WriteRow(const std::vector<std::string> &row){
        for(size_t i = 0; i < row.size(); ++i){
            if(i > 0){
                if(!DDataSink->Put(DDelimiter)){
                    return false;
                }
            }
            
            if(NeedsQuoting(row[i])){
                if(!WriteQuoted(row[i])){
                    return false;
                }
            }
            else{
                for(char ch : row[i]){
                    if(!DDataSink->Put(ch)){
                        return false;
                    }
                }
            }
        }
        
        return DDataSink->Put('\n');
    }
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall){
    DImplementation = std::make_unique<SImplementation>(sink, delimiter, quoteall);
}

CDSVWriter::~CDSVWriter(){
}

bool CDSVWriter::WriteRow(const std::vector<std::string> &row){
    return DImplementation->WriteRow(row);
} 